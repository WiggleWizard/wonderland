#ifndef IPC_COMM_H
#define	IPC_COMM_H

#include <string>
#include <vector>

class IPCCoD4Event;

class IPCComm {
public:
	IPCComm(unsigned int commId, std::string path, std::string prefix);
	IPCComm(const IPCComm& orig);
	virtual ~IPCComm();
	
/*===============================================================*\
 * FUNCTIONS
\*===============================================================*/
	
	/**
	 * Signals the sender thread to send anything on the broadcast
	 * or send stacks.
     */
	void SignalSend();
	
	/**
	 * Receives an entire chunk of data.
	 * 
     * @param socket
     * @param chunk
     * @return 
     */
	char* RecvChunk(int socket, u_int32_t chunkSize);
	
/*===============================================================*\
 * THREADS
\*===============================================================*/
	
	static void* ThreadedConstructor(void* self);
	static void* ThreadedListener(void* self);
	static void* ThreadedSender(void* self);
	
/*===============================================================*\
 * GTORS & STORS
\*===============================================================*/
	
	std::string GetPath();
	
/*===============================================================*\
 * PARSERS
\*===============================================================*/
	
	/**
	 * Parses payloads that are marked with 'V'. See Docs for
	 * packet/payload specs.
	 * 
     * @param payload
     */
	void ParseVoidFunctionPayload(char* payload);
	
/*===============================================================*\
 * VARIABLES
\*===============================================================*/
	
	
	
private:
	pthread_t constructor;
	pthread_t listener;
	pthread_t sender;
	
	pthread_cond_t  sendSig;
	pthread_mutex_t sendLock;
	
	unsigned int commId;
	std::string  path;
	std::string  prefix;
	
	int clientSocket;
	
	std::vector<IPCCoD4Event*> sendQueue;
};

#endif	/* IPC_COMM_H */

