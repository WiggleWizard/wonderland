#include "ipc_event.h"

#include <string>
#include <typeinfo>

#include "../cod4/events.h"

IPCCoD4Event::IPCCoD4Event(char* a1) {
	this->a1 = a1;
}
IPCCoD4Event::IPCCoD4Event(char* a1, char* a2) {
	
}

IPCCoD4Event::IPCCoD4Event(const IPCCoD4Event& orig) {}
IPCCoD4Event::~IPCCoD4Event() {}

/*===============================================================*\
 * COMPILE TEMPLATES
\*===============================================================*/
	
void Compile(unsigned long eventFuncLoc, char* a1, char* a2) {
	
}