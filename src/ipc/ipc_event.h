#ifndef IPC_EVENT_PKT_H
#define	IPC_EVENT_PKT_H

#include <string>
#include <typeinfo>
#include <vector>
#include <mutex>

#include "../globals.h"

class IPCCoD4Event {
public:
	IPCCoD4Event(const char* eventName);

	IPCCoD4Event(const IPCCoD4Event& orig);
	virtual ~IPCCoD4Event();
	
/*===============================================================*\
 * 
\*===============================================================*/
	
	void Compile();
	void AddArgument(void* arg, unsigned int type);

/*===============================================================*\
 * GTORS & STORS
\*===============================================================*/

	bool IsCompiled();

private:
	const char* eventName;
	std::vector<void*> argv;
	std::vector<unsigned int> argt;

	bool compiled;
	std::string packet;

	std::mutex compileLock;
};

#endif	/* IPC_EVENT_PKT_H */

