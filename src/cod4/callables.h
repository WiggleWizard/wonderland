/**
 * Author:  Terence-Lee 'Zinglish' Davis
 * License: MIT
 * Email:   zinglish[at]gmail.com
 * Twitter: @zingatsu
 */

#ifndef HOOK_CALLABLE_H
#define	HOOK_CALLABLE_H

#include <stdint.h>

class Callables {
	// Virtual function locations
	static const unsigned long locfuncGetValueFromSlashString = 0x081AAB50;
	static const unsigned long locfuncEvalCmd                 = 0x08110FF8; // 0x08111BEA
	
	// Virtual variable locations
	static const unsigned long locMaxClients = 0x8370624; // Max clients allowed on the server
	
	// Virtual dvar locations
	// NOTE: These are pointers to a pointer which points to a dvar object.
	//       Inside a dvar object, offset 0 points to the dvar name,
	//       offset 4 is the description and offset 12 is the value (usually a pointer).
	static const unsigned long locMapRotation        = 0x13ED89F8;
	static const unsigned long locMapRotationCurrent = 0x13ED89FC;
	
	// Type definitions
	typedef char*        (*funcdefGetValueFromSlashString)(char* slashedString, char* key);
	typedef unsigned int (*funcdefPlayerForceName)(unsigned long playerId);
	typedef int          (*funcdefEvalCommand)(unsigned long a1, const char* command);
	
	
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
	static int EvalCmd(char* cmd);
	static char* GetMapRotation();
	static char* GetMapRotationCurrent();
private:

};

#endif	/* HOOK_CALLABLE_H */

