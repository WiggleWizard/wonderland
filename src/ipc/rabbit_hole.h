#ifndef IPC_COMM_H
#define	IPC_COMM_H

#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

class IPCCoD4Event;
class IPCReturnFunction;

class RabbitHole {
public:
	RabbitHole(unsigned int commId, std::string path, std::string prefix);
	RabbitHole(const RabbitHole& orig);
	virtual ~RabbitHole();
	
/*===============================================================*\
 * THREADS
\*===============================================================*/
	
	static void* ThreadedConstructor(void* self);
	static void* ThreadedListener(void* self);
	static void* ThreadedSender(void* ipcCommPtr);
	static void* ThreadedEventSender(void* ipcCommPtr);
	static void* ThreadedReturnFunctionSender(void* ipcCommPtr);
	
/*===============================================================*\
 * FUNCTIONS
\*===============================================================*/
	
	/**
	 * Signals the sender thread to send anything on the broadcast
	 * or send stacks.
     */
	void SignalEventSend();
	
	void SignalReturnFunctionSend();
	
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
	bool IsActive();
	
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
	void ParseVoidFunctionPayload(char*& func, std::vector<void*>* argv, std::vector<uint8_t>* argt, char* payload);
	
	
	
/*===============================================================*\
 * VARIABLES
\*===============================================================*/
	
private:
	pthread_t constructor;
	pthread_t listener;
	pthread_t sender;
	
	pthread_cond_t  sendEventSignal;
	pthread_cond_t  sendReturnFunctionSignal;
	std::mutex sendLock;
	std::condition_variable sendRtnFunctionCond;
	pthread_mutex_t sendEventLock;
	pthread_mutex_t sendReturnFunctionLock;
	std::mutex returnFunctionsModLock;
	
	unsigned int commId;
	std::string  path;
	std::string  prefix;
	
	int clientSocket;
	int listenSocket;
	bool active;
	
	std::vector<IPCCoD4Event*>      queueEvents;
	std::vector<IPCReturnFunction*> queueReturnFunctions;
};

#endif	/* IPC_COMM_H */

