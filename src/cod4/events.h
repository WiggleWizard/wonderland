#ifndef HOOK_EVENT_H
#define	HOOK_EVENT_H

#include <vector>

class Hook;

class Events {
	static const unsigned long locPlayerSay        = 0x080AE962;
	static const unsigned long locPlayerNameChange = 0x081705EC;
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
	
/*===============================================================*\
 * HOOK VARIABLES
\*===============================================================*/
	
	static Hook* hPlayerSay;
	static Hook* hPlayerChangeName;
	
private:
	static std::vector<unsigned long> eventIds;
};

#endif	/* HOOK_EVENT_H */

