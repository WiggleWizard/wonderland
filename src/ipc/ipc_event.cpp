#include "ipc_event.h"

#include <string>
#include <cstring>
#include <typeinfo>

#include "../cod4/events.h"

IPCCoD4Event::IPCCoD4Event(const char* eventName)
{
	this->compiled = false;

	this->eventName = eventName;
	this->argv      = std::vector<void*>(5);
	this->argt      = std::vector<unsigned int>(5);
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
	
void IPCCoD4Event::Compile()
{
	std::lock_guard<std::mutex> lock(this->compileLock);

	unsigned int payloadSize = 0;
	unsigned int args        = this->argv.size();

	// We first estimate how total length of the payload
	payloadSize += 1; // Byte for packet type
	payloadSize += 4; // For the size of the command
	payloadSize += strlen(this->eventName);

	for(unsigned int i = 0; i < args; i++)
	{
		// We check the type then cast it if needed and add to the
		// payload size
		if(this->argt.at(i) == ipcTypes.uint || this->argt.at(i) == ipcTypes.sint)
			payloadSize += 4;
		else if(this->argt.at(i) == ipcTypes.ch)
			payloadSize += strlen((char*) this->argv.at(i));
	}

	// Prepare the packet
	this->packet.reserve(4 + payloadSize);
	

	// Now we construct the packet
	for(unsigned int i = 0; i < args; i++)
	{
	}

	this->compiled = true;
}

/*===============================================================*\
 * GTORS & STORS
\*===============================================================*/

bool IPCCoD4Event::IsCompiled()
{
	return this->compiled;
}