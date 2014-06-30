/*
 * @author: Terence-Lee 'Zinglish' Davis
 * 
 * Designed to handle UNIX sockets and its clients.
 */

#ifndef SERVER_H
#define	SERVER_H

#include <string>
#include <vector>

#include "ipc_event.h"

class IPCComm;
class IPCCoD4Event;

class IPCServer {
public:
	/**
	 * Constructs a listener UNIX socket at `path` and starts a new thread.
	 * 
     * @param path UNIX socket path.
     */
	IPCServer(std::string path);
	
	IPCServer(const IPCServer& orig);
	virtual ~IPCServer();
	
/*===============================================================*\
 * THREADS
\*===============================================================*/
	
	/**
	 * Separate thread to monitor incoming standard transmissions.
	 * 
     * @param serverPtr Usually described as `this`.
     * @return 
     */
	static void* ThreadedCommAllocator(void* serverPtr);
	
	/**
	 * Thread deals with direct communications with the CoD4 service.
	 * 
     * @param serverPtr
     * @return 
     */
	static void* ThreadedComm(void* serverPtr);
	
/*===============================================================*\
 * FUNCTIONS
\*===============================================================*/
	
	/**
	 * Assigns/Creates a new communication path.
	 * 
     * @return Newly created communication path/name.
     */
	unsigned int CreateNewComm();
	
	/**
	 * Parses receiving transmissions that occur in the manager.
	 * 
     * @param pkt Packet from the manager.
     * @return Packet to send back to the client.
     */
	char* ParseManagerRXPacket(char* pkt);
	
	/**
	 * Adds an event broadcast on to the queue then signals all outbound
	 * comms to send the event to Alice.
	 * 
     * @param event
     */
	static void BroadcastEvent(IPCCoD4Event* event);

/*===============================================================*\
 * VARIABLES
\*===============================================================*/
	
	// Tracks client comms
	static std::vector<IPCComm*> clientComms;
	
private:
	pthread_t listener;
	
	std::string clientCommPrefix;
	std::string clientCommPath;
	
	// Holds event type packets which should be broadcasted across
	// all connected clients.
	static std::vector<IPCCoD4Event*> bcastEvents;
};

#endif	/* SERVER_H */

