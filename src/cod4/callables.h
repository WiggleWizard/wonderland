#ifndef HOOK_CALLABLE_H
#define	HOOK_CALLABLE_H

#include <stdint.h>

class Callables {
	// Virtual variable locations
	static const unsigned long locMaxClients = 0x8370624; // Max clients allowed on the server
	
public:
	Callables();
	Callables(const Callables& orig);
	virtual ~Callables();
	
	static uint32_t GetPlayerIdByOffset(uint32_t offset);
	static void BroadcastChat(char* message);
	static uint32_t GetMaxClients();
private:

};

#endif	/* HOOK_CALLABLE_H */

