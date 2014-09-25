#ifndef IPC_EVENT_PKT_H
#define	IPC_EVENT_PKT_H

#include <string>
#include <typeinfo>
#include <vector>

class IPCCoD4Event {
	static const char delim = 0x01;
public:
	IPCCoD4Event(const char* eventName);

	IPCCoD4Event(const IPCCoD4Event& orig);
	virtual ~IPCCoD4Event();
	
/*===============================================================*\
 * 
\*===============================================================*/
	
	void Compile();
	void AddArgument(void* arg, unsigned int type);
private:
	char* eventName;
	std::vector<void*> argv;
	std::vector<unsigned int> argt;

	std::string packetString;
};

#endif	/* IPC_EVENT_PKT_H */

