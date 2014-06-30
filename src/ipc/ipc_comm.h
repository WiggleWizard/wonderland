#ifndef IPC_COMM_H
#define	IPC_COMM_H

#include <string>
#include <vector>

class IPCComm {
public:
	IPCComm(unsigned int commId, std::string path, std::string prefix);
	IPCComm(const IPCComm& orig);
	virtual ~IPCComm();
	
/*===============================================================*\
 * FUNCTIONS
\*===============================================================*/
	
	void SignalSend();
	
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
	
	std::vector<char*> sendQueue;
};

#endif	/* IPC_COMM_H */

