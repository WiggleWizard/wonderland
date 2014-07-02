#include "ipc_event.h"

#include <string>
#include <typeinfo>

#include "../cod4/events.h"

IPCCoD4Event::IPCCoD4Event(unsigned int eventId, unsigned int a1type, char* a1) {
	this->eventId = eventId;
	
	this->a1 = a1;
}
IPCCoD4Event::IPCCoD4Event(unsigned int eventId, char* a1, char* a2) {
	this->eventId = eventId;
	
	this->a1 = a1;
	this->a2 = a2;
}

IPCCoD4Event::IPCCoD4Event(const IPCCoD4Event& orig) {}
IPCCoD4Event::~IPCCoD4Event() {}

/*===============================================================*\
 * COMPILE TEMPLATES
\*===============================================================*/
	
void Compile() {
	
}