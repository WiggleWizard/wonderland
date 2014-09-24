#include "ipc_server_man.h"
#include "ipc_comm.h"
#include "../globals.h"

#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

std::vector<IPCComm*> IPCServer::clientComms;

IPCServer::IPCServer(std::string path) {
	// Initialize the client holder
	this->clientComms.reserve(5);
	
	this->clientCommPrefix = "comm-";
	this->clientCommPath   = "/tmp/";
	
	// Create the management thread
	pthread_create(&this->listener, NULL, &IPCServer::ThreadedCommAllocator, this);
}

IPCServer::IPCServer(const IPCServer& orig) {}
IPCServer::~IPCServer() {}

/*===============================================================*\
 * THREADS
\*===============================================================*/

/**
 * Manages incoming link requests to the server. If the client is allowed to then
 * the thread will construct a new link + thread to communicate fully with Alice.
 */
void* IPCServer::ThreadedCommAllocator(void* serverPtr) {
	IPCServer* server = (IPCServer*) serverPtr;
	
	int localSock;
	struct sockaddr_un local;

	// Create the socket
	if((localSock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		printf("Socket creation failed, error: %i\n", errno);
		exit(1);
	}
	
	// Prepare and bind the socket
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, "/tmp/wonderland");
	unlink(local.sun_path); // Delete the original socket
	unsigned int len = strlen(local.sun_path) + sizeof(local.sun_family);
	
	if(bind(localSock, (struct sockaddr *)&local, len) == -1) {
		printf("Binding failed, error: %i\n", errno);
		exit(1);
	}

	// Start listening
	if (listen(localSock, 5) == -1) {
		printf("Listen failed, error: %i\n", errno);
		exit(1);
	}
        
	printf("IPC (v%i) server started\n", IPC_VER);
	
	// Accept connections indefinitely
	while(true) {
		printf("Awaiting new connection\n");
		
		struct sockaddr_un remote;
		int clientSock;
		unsigned int clientSockSize = sizeof(remote);
		
		// Hang until a connection is incoming
		if((clientSock = accept(localSock, (struct sockaddr *)&remote, &clientSockSize)) == -1) {
			printf("Accept failed, error: %i\n", errno);
			exit(1);
		}
		
		printf("Connection established\n");
		
		// Once connected we need to get data from the client
		char rxRaw[8]; // 8bytes (1st int is version, 2nd int is size of payload)
		std::string rxString = ""; // Used for debugging
		unsigned int rxSize = sizeof(rxRaw);
		int rxStatus = 0;
		
		char* payload = NULL;
		
		while(true) {
			rxStatus = recv(clientSock, &rxRaw, 8, 0);
			
			// An error occurred -OR- EOS
			if(rxStatus <= 0)
				break;
			
			// Collate the data for debugging
			//rxString.append(rxRaw, (u_int32_t) rxRaw[3]);

			// Get the version and payload length
			u_int32_t version    = ntohl(*(u_int32_t*) &rxRaw[0]);
			u_int32_t payloadLen = ntohl(*(u_int32_t*) &rxRaw[4]);

			// Get the full payload
			payload = server->RecvPayload(clientSock, payloadLen);
			
			// General debugging
			printf("Version: %i\n", version);
			printf("Payload length: %i\n", payloadLen);
			printf("Payload: %s\n", payload);
		}
		
		delete payload;

		printf("Client disconnected\n");
	}
}



/*===============================================================*\
 * FUNCTIONS
\*===============================================================*/

char* IPCServer::RecvPayload(int socket, u_int32_t payloadSize)
{
	// Make space for the payload, setting the last byte to 0x00 as if
	// it was a C style string
	char* payload = new char[payloadSize + 1];
	memset(&payload[payloadSize], 0x00, 1);
	
	unsigned int curPos     = 0;
	unsigned int currLen    = 0; // Amount of chars written to payload
	unsigned int bufferSize = 3; // Recv 64bytes every read
	
	while(curPos < payloadSize)
	{
		currLen = payloadSize - curPos;

		if(currLen < bufferSize)
			bufferSize = currLen;

		recv(socket, &payload[curPos], bufferSize, 0);
		
		curPos += bufferSize;
	}
	
	return payload;
}

char* IPCServer::ParseManagerRXPacket(char* pkt) {
	char* rtn = NULL;
	
	// If it's a request to initiate a full communication socket
	if(pkt[0] == 0x01 && pkt[1] == 0x00)
	{
		// Prepare the full path to the new comm
		unsigned int commId = this->CreateNewComm();
		
		std::string fullCommPath = clientComms.at(commId)->GetPath();
		rtn = (char*) malloc(fullCommPath.length());
		memcpy(rtn, fullCommPath.c_str(), fullCommPath.length());
	}
	
	return rtn;
}

unsigned int IPCServer::CreateNewComm() {
	// Prepare args for creating a new comm
	unsigned int commId = this->clientComms.size();
	
	// Create a new comm and push it into the array
	this->clientComms.push_back(new IPCComm(commId, this->clientCommPath, this->clientCommPrefix));
	
	return commId;
}

void IPCServer::BroadcastEvent(IPCCoD4Event* event) {
	// Signal all comm channels
	for(unsigned int i=0; i<IPCServer::clientComms.size(); i++) {
		IPCComm* commChan = IPCServer::clientComms[i];
		commChan->SignalSend();
	}
}