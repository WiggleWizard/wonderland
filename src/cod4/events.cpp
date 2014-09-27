#include "events.h"

#include "hook.h"
#include "callables.h"
#include "../globals.h"
#include "../ipc/ipc_event.h"
#include "../ipc/ipc_server_man.h"

Hook* Events::hPlayerSay;

Events::Events() {}
Events::Events(const Events& orig) {}
Events::~Events() {}

void Events::InsertHooks()
{
	Events::hPlayerSay = new Hook((void*) Events::locPlayerSay, 5, (void*) Events::HPlayerSay);
}

/*===============================================================*\
 * EVENTS
\*===============================================================*/

/**
* Triggered when the player says something
* This function also bypasses the script if !login command is issued on the server
* @param a1 Pointer to the inet object
* @param a2
* @param a3 Sayteam produces 1 and say produces 0
* @param a4 Pointer to the character for message
* @return 
*/
int Events::HPlayerSay(unsigned int* playerId, int a2, int teamSay, char* message)
{
	// Prepare the arguments and types for the event
	int* argTeamSay = new int;
	*argTeamSay = teamSay;

	unsigned int messageLen = strlen(message);
	char* argMessage = new char[messageLen + 1];
	memcpy(argMessage, message, messageLen);
	argMessage[messageLen] = '\0';

	// Collate the arguments and types
	char* cmd = new char[4 + 1];
	strcpy(cmd, "CHAT");
	IPCCoD4Event* ipcEvent = new IPCCoD4Event(cmd);
	ipcEvent->AddArgument((void*) 5, IPCTypes::uint);
	ipcEvent->AddArgument((void*) argMessage, IPCTypes::ch);
	

	// Broadcast the event
	IPCServer::SetEventForBroadcast(ipcEvent);
	
	printf("[%i]: %s\n", *playerId, message);

	return 0;
}