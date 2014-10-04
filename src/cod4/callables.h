#ifndef HOOK_CALLABLE_H
#define	HOOK_CALLABLE_H

#include <stdint.h>

class Callables {
	// Virtual function locations
	static const unsigned long locfuncGetValueFromSlashString = 0x081AAB50;
	
	// Virtual variable locations
	static const unsigned long locMaxClients = 0x8370624; // Max clients allowed on the server
	
	// Type definitions
	typedef char*        (*funcdefGetValueFromSlashString)(char* slashedString, char* key);
	typedef unsigned int (*funcdefPlayerForceName)(unsigned long playerId);
	
	
public:
	Callables();
	Callables(const Callables& orig);
	virtual ~Callables();
	
	static uint32_t GetPlayerIdByOffset(uint32_t offset);
	static void BroadcastChat(char* message);
	static uint32_t GetMaxClients();
	static char* GetValueFromSlashString(char* slashedString, char* key);
	static void SetPlayerName(unsigned int playerId, char* name);
	static void TellPlayer(unsigned int playerId, char* message);
private:

};

#endif	/* HOOK_CALLABLE_H */

