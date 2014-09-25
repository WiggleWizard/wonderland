#include "ipc_event.h"

#include <string>
#include <cstring>
#include <typeinfo>
#include <netinet/in.h>

#include "../cod4/events.h"
#include "../globals.h"

IPCCoD4Event::IPCCoD4Event(const char* eventName)
{
	this->compiled = false;
	this->packet   = NULL;

	this->eventName = eventName;
	this->argv      = std::vector<void*>(5);
	this->argt      = std::vector<unsigned int>(5);
}

IPCCoD4Event::~IPCCoD4Event()
{
	// Free up memory
	unsigned int args = this->argv.size();
	for(unsigned int i = 0; i < args; i++)
	{
		delete this->argv.at(i);
	}
	
	delete this->eventName;
	delete this->packet;
}

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
	
	// Clean up memory if exists
	if(this->packet != NULL)
		delete this->packet;

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
		if(this->argt.at(i) == IPCTypes::uint || this->argt.at(i) == IPCTypes::sint)
			payloadSize += 4;
		else if(this->argt.at(i) == IPCTypes::ch)
			payloadSize += strlen((char*) this->argv.at(i));
	}
	
	this->packetLen = 4 + payloadSize;

	// Prepare the packet
	this->packet = new char[4 + payloadSize];
	
	u_int32_t s = htonl(payloadSize); // Payload size
	memcpy(this->packet, &s, 4);
	this->packet[4] = 'E';            // Packet type
	memcpy(this->packet + 5);

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

char* IPCCoD4Event::GetPacket()
{
	if(!this->compiled)
		this->Compile();
	
	return this->packet;
}

unsigned int IPCCoD4Event::GetPacketSize()
{
	return this->packetLen;
}