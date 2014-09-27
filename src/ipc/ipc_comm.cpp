#include "ipc_comm.h"

#include "ipc_event.h"
#include "ipc_server_man.h"

#include <string>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

IPCComm::IPCComm(unsigned int commId, std::string path, std::string prefix) {
	this->commId = commId;
	this->path   = path;
	this->prefix = prefix;
	
	// Initialize the locks and conditions for the threads
	this->sendLock = PTHREAD_MUTEX_INITIALIZER;
	this->sendSig  = PTHREAD_COND_INITIALIZER;
	
	// Create the listening thread
	pthread_create(&this->constructor, NULL, IPCComm::ThreadedConstructor, this);
}

IPCComm::~IPCComm() {}

/*===============================================================*\
 * THREADS
\*===============================================================*/

void* IPCComm::ThreadedConstructor(void* ipcCommPtr)
{
	IPCComm* self = (IPCComm*) ipcCommPtr;
	
	int localSock;
	struct sockaddr_un local;

	// Create the socket
	if((localSock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		printf("Socket creation failed, error: %i\n", errno);
		exit(1);
	}
	
	// Prepare and bind the socket
	local.sun_family = AF_UNIX;
	std::string commPath = self->GetPath();
	commPath.copy(local.sun_path, commPath.length(), 0);
	unlink(local.sun_path); // Delete the original socket
	unsigned int len = strlen(local.sun_path) + sizeof(local.sun_family);
	
	if(bind(localSock, (struct sockaddr *)&local, len) == -1)
	{
		printf("Binding failed, error: %i\n", errno);
		exit(1);
	}

	// Start listening
	if(listen(localSock, 5) == -1)
	{
		printf("Listen failed, error: %i\n", errno);
		exit(1);
	}
	
	printf("Rabbit hole %i initialized\n", self->commId);
		
	struct sockaddr_un remote;
	unsigned int clientSockSize = sizeof(remote);

	// Accept the first connection request
	if((self->clientSocket = accept(localSock, (struct sockaddr *)&remote, &clientSockSize)) == -1)
	{
		printf("Accept failed, error: %i\n", errno);
		exit(1);
	}
	
	printf("Rabbit hole %i connection made\n", self->commId);
	
	// Create the listening thread
	pthread_create(&self->listener, NULL, IPCComm::ThreadedListener, self);
	
	// Create the sending thread
	pthread_create(&self->sender, NULL, &IPCComm::ThreadedSender, self);
}

void* IPCComm::ThreadedListener(void* ipcCommPtr)
{
	IPCComm* self = (IPCComm*) ipcCommPtr;
	
	while(true)
	{
		char seq[5];
		int rxStatus = 0;
		u_int32_t payloadLen = 0;
		uint8_t packetType;
		
		char* payload = NULL;
		
		// All incoming packets will be prefixed with 5 bytes of important
		// information.
		rxStatus = recv(self->clientSocket, &seq, 5, 0);
		
		// An error occurred -OR- EOS
		if(rxStatus <= 0)
			break;
		
		// Retrieve data from the packet
		packetType = seq[0];
		payloadLen = ntohl(*(u_int32_t*) &seq[1]);
		
		if(packetType == 'V')
		{
			// Get the entire payload
			payload = self->RecvChunk(self->clientSocket, payloadLen);
			
			// Prep variables to be modified by the parse
			char* func                 = NULL;
			std::vector<void*>* argv   = new std::vector<void*>;
			std::vector<uint8_t>* argt = new std::vector<uint8_t>;
			
			self->ParseVoidFunctionPayload(func, argv, argt, payload);
			
			delete payload;
		}
	}
}

/**
 * Threaded sender, when signalled it will send the packets ontop of the queue
 * @param ipcCommPtr
 * @return 
 */
void* IPCComm::ThreadedSender(void* ipcCommPtr)
{
	IPCComm* self = (IPCComm*) ipcCommPtr;
	
	while(true)
	{
		// The thread will only continue once the signal has been triggered
		pthread_cond_wait(&self->sendSig, &self->sendLock);
		
		// Compile and send each Event
		IPCCoD4Event* event = NULL;
		unsigned int s = IPCServer::broadcastEvents.size();
		for(unsigned int i = 0; i < s; i++)
		{
			event = IPCServer::broadcastEvents.at(i);
			
			// Compile and send
			event->Compile();
			send(self->clientSocket, event->GetPacket(), event->GetPacketSize(), 0);

			// Adds 1 to the sent counter
			event->Sent();

			// Double checks the sent counter, if it's equal to the same
			// amount of clients running then it's disposed of
			if(event->SentTimes() >= IPCServer::clientComms.size())
				IPCServer::DestroyEvent(event);
		}
		event = NULL;
	}

	return NULL;
}

/*===============================================================*\
 * FUNCTIONS
\*===============================================================*/

void IPCComm::SignalSend()
{
	pthread_cond_signal(&this->sendSig);
}

char* IPCComm::RecvChunk(int socket, u_int32_t chunkSize)
{
	// Make space for the payload, setting the last byte to 0x00 as if
	// it was a C style string
	char* payload = new char[chunkSize + 1];
	
	unsigned int curPos     = 0;
	unsigned int currLen    = 0; // Amount of chars written to payload
	unsigned int bufferSize = 8; // Recv 8 bytes every read
	
	while(curPos < chunkSize)
	{
		currLen = chunkSize - curPos;

		// Set the recv size under certain circumstances
		if(currLen < bufferSize)
			bufferSize = currLen;
		if(chunkSize < bufferSize)
			bufferSize = chunkSize;

		recv(socket, &payload[curPos], bufferSize, 0);
		
		curPos += bufferSize;
	}
	
	payload[chunkSize] = '\0';
	
	return payload;
}

/*===============================================================*\
 * PARSERS
\*===============================================================*/

void IPCComm::ParseVoidFunctionPayload(char* func, std::vector<void*>* argv, std::vector<uint8_t>* argt, char* payload)
{
	unsigned int cursor = 0;
	
	// - Function name size
	unsigned int funcSize = ntohl(*(uint32_t*) payload);
	cursor += 4;
	
	// - Function name
	func = new char[funcSize + 1];
	func[funcSize] = '\0';
	memcpy(func, payload + cursor, funcSize);
	cursor += funcSize;
	
	// - Arg count
	uint8_t argc = *(uint8_t*) (payload + cursor);
	cursor += 1;
	
	// - Args
	argv->reserve(argc);
	argt->reserve(argc);
	for(unsigned int i = 0; i < argc; i++)
	{
		// -- Type
		argt->push_back(*(uint8_t*) (payload + cursor));
		cursor += 1;
		
		// -- Value
		if((*argt)[i] == IPCTypes::uint || (*argt)[i] == IPCTypes::sint)
		{
			// We first have to convert the network byte order integer to host
			// order then make space for it in memory so we can keep it for
			// later use.
			uint32_t integer = ntohl(*(uint32_t*) (payload + cursor));
			char* intSpace = new char[4];
			
			memcpy(intSpace, &integer, 4);
			argv->push_back(intSpace);
			
			cursor += 4;
		}
		else if((*argt)[i] == IPCTypes::ch)
		{
			// --- Size of string
			uint32_t s = ntohl(*(uint32_t*) (payload + cursor));
			cursor += 4;
			
			// --- String value
			char* str = new char[s + 1];
			memcpy(str, payload + cursor, s);
			str[s] = '\0';
			argv->push_back(str);
			
			cursor += s;
		}
	}
}

/*===============================================================*\
 * GTORS & STORS
\*===============================================================*/

std::string IPCComm::GetPath() {
	return std::string(this->path) + std::string(this->prefix) + std::to_string(this->commId);
}