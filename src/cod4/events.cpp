#include "events.h"

#include "hook.h"
#include "callables.h"
#include "../globals.h"
#include "../ipc/ipc_event.h"
#include "../ipc/ipc_server_man.h"

#include "player.h"

Hook* Events::hPlayerSay;
Hook* Events::hPlayerChangeName;

Events::Events() {}
Events::Events(const Events& orig) {}
Events::~Events() {}

void Events::InsertHooks()
{
	Events::hPlayerSay        = new Hook((void*) Events::locPlayerSay, 5, (void*) Events::HPlayerSay);
	Events::hPlayerChangeName = new Hook((void*) Events::locPlayerNameChange, 5, (void*) Events::HPlayerNameChange);
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
	//int* argTeamSay = new int;
	//*argTeamSay = teamSay;
	
	Player player(*playerId);
	unsigned int playerNameLen = strlen(player.GetName());
	char* playerName = new char[playerNameLen + 1];
	memcpy(playerName, player.GetName(), playerNameLen);
	playerName[playerNameLen] = '\0';

	unsigned int messageLen = strlen(message);
	char* argMessage = new char[messageLen + 1];
	memcpy(argMessage, message, messageLen);
	argMessage[messageLen] = '\0';

	// Collate the arguments and types
	char* cmd = new char[4 + 1];
	strcpy(cmd, "CHAT");
	IPCCoD4Event* ipcEvent = new IPCCoD4Event(cmd);
	ipcEvent->AddArgument((void*) *playerId, IPCTypes::uint);
	ipcEvent->AddArgument((void*) playerName, IPCTypes::ch);
	ipcEvent->AddArgument((void*) argMessage, IPCTypes::ch);
	
	printf("%s: %s\n", playerName, message);

	// Broadcast the event
	IPCServer::SetEventForBroadcast(ipcEvent);

	return 0;
}

int Events::HPlayerNameChange(unsigned int playerOffset)
{
	unsigned int playerId = Callables::GetPlayerIdByOffset(playerOffset);
	char* newNameRtn = Callables::GetValueFromSlashString((char*)(0x0887C320 + 9), "name");
	
	// Copy the name into a new char
	unsigned int newNameLen = strlen(newNameRtn);
	char* newName = new char[newNameLen + 1];
	memcpy(newName, newNameRtn, newNameLen);
	newName[newNameLen] = '\0';
	
	char* cmd = new char[10 + 1];
	strcpy(cmd, "CHANGENAME");
	IPCCoD4Event* ipcEvent = new IPCCoD4Event(cmd);
	ipcEvent->AddArgument((void*) playerId, IPCTypes::uint);
	ipcEvent->AddArgument((void*) newName, IPCTypes::ch);
	
	// Broadcast the event
	IPCServer::SetEventForBroadcast(ipcEvent);
	
	return 0;
}