#include "ipc_return_function.h"

#include "../globals.h"

#include <netinet/in.h>
#include <cstring>

IPCReturnFunction::IPCReturnFunction()
{
}

IPCReturnFunction::IPCReturnFunction(const IPCReturnFunction& orig) {}
IPCReturnFunction::~IPCReturnFunction()
{
	// Free up arg memory
	unsigned int args = this->argv.size();
	for(unsigned int i = 0; i < args; i++)
	{
		if(this->argt[i] == IPCTypes::uint || this->argt[i] == IPCTypes::sint)
			delete (uint32_t*) this->argv[i];
		else if(this->argt[i] == IPCTypes::ch)
			delete [] (char*) this->argv[i];
	}
	
	delete [] this->functionName;
	
	// Decide how to delete the return based on its stored type
	if(this->functionReturnType == IPCTypes::uint || this->functionReturnType == IPCTypes::sint)
		delete (uint32_t*) this->functionReturnPtr;
	else if(this->functionReturnType == IPCTypes::ch)
		delete [] (char*) this->functionReturnPtr;
}

void IPCReturnFunction::Parse(char* payload, bool destructive)
{
	unsigned int cursor = 0;
	
	// - Packet ID
	this->packetID = ntohl(*(uint32_t*) payload);
	cursor += 4;
	
	// - Function name size
	unsigned int funcSize = ntohl(*(uint32_t*) payload + cursor);
	cursor += 4;
	
	// - Function name
	this->functionName = new char[funcSize + 1];
	this->functionName[funcSize] = '\0';
	memcpy(this->functionName, payload + cursor, funcSize);
	cursor += funcSize;
	
	// - Arg count
	uint8_t argc = *(uint8_t*) (payload + cursor);
	cursor += 1;
	
	// - Args
	this->argv.reserve(argc);
	this->argt.reserve(argc);
	for(unsigned int i = 0; i < argc; i++)
	{
		// -- Type
		this->argt.push_back(*(uint8_t*) (payload + cursor));
		cursor += 1;
		
		// -- Value
		if(this->argt[i] == IPCTypes::uint || this->argt[i] == IPCTypes::sint)
		{
			// We first have to convert the network byte order integer to host
			// order then make space for it in memory so we can keep it for
			// later use.
			uint32_t integer = ntohl(*(uint32_t*) (payload + cursor));
			char* intSpace = new char[4];
			
			memcpy(intSpace, &integer, 4);
			this->argv.push_back(intSpace);
			
			cursor += 4;
		}
		else if(this->argt[i] == IPCTypes::ch)
		{
			// --- Size of string
			uint32_t s = ntohl(*(uint32_t*) (payload + cursor));
			cursor += 4;
			
			// --- String value
			char* str = new char[s + 1];
			memcpy(str, payload + cursor, s);
			str[s] = '\0';
			this->argv.push_back(str);
			
			cursor += s;
		}
	}
}