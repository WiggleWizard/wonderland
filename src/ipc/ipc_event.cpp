#include "ipc_event.h"

#include <string>
#include <typeinfo>

#include "../cod4/events.h"

IPCCoD4Event::IPCCoD4Event(const char* eventName)
{
	this->argv = std::vector<void*>(3);
	this->argt = std::vector<unsigned int>(3);
}

IPCCoD4Event::IPCCoD4Event(const IPCCoD4Event& orig) {}
IPCCoD4Event::~IPCCoD4Event() {}

/*===============================================================*\
 * 
\*===============================================================*/

void IPCCoD4Event::AddArgument(void* arg, unsigned int type)
{
	this->argv.push_back(arg);
	this->argt.push_back(type);
}
	
void Compile() {
	
}