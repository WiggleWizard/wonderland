#include "ipc_comm.h"

#include "ipc_event.h"
#include "ipc_server_man.h"

#include <string>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

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
 * FUNCTIONS
\*===============================================================*/

void IPCComm::SignalSend() {
	pthread_cond_signal(&this->sendSig);
}

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
}

/**
 * Threaded sender, when signalled it will send the packets ontop of the queue
 * @param ipcCommPtr
 * @return 
 */
void* IPCComm::ThreadedSender(void* ipcCommPtr) {
	IPCComm* self = (IPCComm*) ipcCommPtr;
	
	while(true) {
		// The thread will only continue once the signal has been triggered
		pthread_cond_wait(&self->sendSig, &self->sendLock);
		
		// Compile and send each IPC Event
		IPCCoD4Event* event = NULL;
		unsigned int s = IPCServer::broadcastEvents.size();
		for(unsigned int i = 0; i < s; i++)
		{
			event = IPCServer::broadcastEvents.at(i);
			
			// GetPacket() compiles and returns the char* to the packet
			send(self->clientSocket, event->GetPacket(), 5, 0);
		}
		event = NULL;
	}
}

/*===============================================================*\
 * GTORS & STORS
\*===============================================================*/

std::string IPCComm::GetPath() {
	return std::string(this->path) + std::string(this->prefix) + std::to_string(this->commId);
}