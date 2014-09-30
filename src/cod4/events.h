#ifndef HOOK_EVENT_H
#define	HOOK_EVENT_H

#include <vector>

class Hook;

class Events {
	static const unsigned long locPlayerSay        = 0x080AE962;
	static const unsigned long locPlayerNameChange = 0x081705EC;
	static const unsigned long locRconStatus       = 0x0816C708;
public:
	Events();
	Events(const Events& orig);
	virtual ~Events();
	
/*===============================================================*\
 * FUNCTIONS
\*===============================================================*/
	
	static void InsertHooks();
	
/*===============================================================*\
 * EVENTS
\*===============================================================*/
	
	static int HPlayerSay(unsigned int* playerId, int a2, int teamSay, char* message);
	static int HPlayerNameChange(unsigned int playerOffset);
	static int HServerStatusRequest();
	
/*===============================================================*\
 * HOOK VARIABLES
\*===============================================================*/
	
	static Hook* hPlayerSay;
	static Hook* hPlayerChangeName;
	static Hook* hServerStatusRequest;
	
private:
	static std::vector<unsigned long> eventIds;
};

#endif	/* HOOK_EVENT_H */

