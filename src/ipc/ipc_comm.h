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
 * THREADS
\*===============================================================*/
	
	static void* ThreadedConstructor(void* self);
	static void* ThreadedListener(void* self);
	static void* ThreadedSender(void* self);
	
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
	
	/**
	 * Attempts execution of func. Ensure you clean up func, argv and argt
	 * after execution of this function.
	 * 
     * @param func Function name.
     * @param argv Vector of argument values.
     * @param argt Vector of argument types.
     */
	void ExecVoidFunction(char* func, std::vector<void*>* argv, std::vector<uint8_t>* argt);
	
/*===============================================================*\
 * GTORS & STORS
\*===============================================================*/
	
	std::string GetPath();
	
/*===============================================================*\
 * PARSERS
\*===============================================================*/
	
	/**
	 * Parses payloads marked with 'V'. See the docs for the payload
	 * specs.
	 * 
     * @param func Function name (Modified at runtime, init as NULL).
     * @param argv Vector of argument values (Init as 'new').
     * @param argt Vector of argument types (Init as 'new').
     * @param payload Payload to be parsed.
     */
	void ParseVoidFunctionPayload(char* func, std::vector<void*>* argv, std::vector<uint8_t>* argt, char* payload);
	
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

