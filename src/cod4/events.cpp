#include "events.h"

#include "hook.h"
#include "callables.h"
#include "../globals.h"
#include "../ipc/ipc_event.h"
#include "../ipc/ipc_server_man.h"
#include "player.h"

#include <arpa/inet.h>

Hook* Events::hPlayerJoinRequest;
Hook* Events::hPlayerSay;
Hook* Events::hPlayerChangeName;
Hook* Events::hServerStatusRequest;

Events::Events() {}
Events::Events(const Events& orig) {}
Events::~Events() {}

void Events::InsertHooks()
{
	Events::hPlayerJoinRequest   = new Hook((void*) Events::locPlayerJoinRequest, 5, (void*) Events::HPlayerJoinRequest);
	Events::hPlayerSay           = new Hook((void*) Events::locPlayerSay, 5, (void*) Events::HPlayerSay);
	Events::hPlayerChangeName    = new Hook((void*) Events::locPlayerNameChange, 5, (void*) Events::HPlayerNameChange);
	Events::hServerStatusRequest = new Hook((void*) Events::locRconStatus, 5, (void*) Events::HServerStatusRequest);
}

/*===============================================================*\
 * EVENTS
\*===============================================================*/

bool Events::HPlayerJoinRequest(unsigned long a1, uint32_t ip, unsigned long a3, unsigned long a4, unsigned long a5)
{
	// Predict slot ID
	unsigned int  slotID       = 0;
	unsigned long slotOffset   = 0x090B4F8C;
	unsigned int  assignedSlot = 0;
	
	unsigned int maxClients = Callables::GetMaxClients();
	for(uint32_t i = 0x090B4F8C; ;i += 677436)
	{
		if(slotID > maxClients)
			break;
		
		slotID = 35580271 * ((i - 151736204) >> 2);
		
		// If the slot's disconnected
		if(*(uint32_t*) i == 0)
		{
			slotOffset = i;
			
			printf("Slot [%i] is going to be used is currently in state: %i\n", slotID, *(uint32_t*) slotOffset);
			
			break;
		}
		
		//bool r = ((Events::funcdefIsPlayerConnectedAtSlot)Events::locfuncIsPlayerConnectedAtSlot)
					//(a1, ip, a3, a4, a5, *(uint32_t *)(i + 32), *(uint32_t *)(i + 36), *(uint32_t *)(i + 40));

		// If the slot was free
		//if(r == false)
			//break;
	}
	
	// Get the string value out of the IP
	struct in_addr ip_addr;
	ip_addr.s_addr = ip;
	char* addr = inet_ntoa(ip_addr);
	
	// Copy the IP pointer into a character pointer, this is required because
	// the char* 'addr' is actually a pointer to a space inside the in_addr
	// object, so it will be cleaned once the ip_addr object goes out of scope.
	char* ipAddress = new char[strlen(addr) + 1];
	memcpy(ipAddress, addr, strlen(addr));
	ipAddress[strlen(addr)] = '\0';
	
	char* cmd = new char[9 + 1];
	strcpy(cmd, "JOINREQ");
	IPCCoD4Event* ipcEvent = new IPCCoD4Event(cmd);
	ipcEvent->AddArgument((void*) ipAddress, IPCTypes::ch);
	
	// Broadcast the event
	IPCServer::SetEventForBroadcast(ipcEvent);
	
	// Execute the origional request to join the server
	Events::hPlayerJoinRequest->UnHook();
	bool rtn = ((Events::funcdefPlayerJoinRequest)Events::locPlayerJoinRequest)(a1, ip, a3, a4, a5);
	Events::hPlayerJoinRequest->Rehook();
	
	// After the request is executed, we will check if the connection was a success
	// then fire another event.
	Player player(slotID);
	if(player.GetConnState() > 0)
	{
		char* ipAddress = new char[strlen(addr) + 1];
		memcpy(ipAddress, addr, strlen(addr));
		ipAddress[strlen(addr)] = '\0';
		
		unsigned int playerNameLen = strlen(player.GetName());
		char* playerName = new char[playerNameLen + 1];
		memcpy(playerName, player.GetName(), playerNameLen);
		playerName[playerNameLen] = '\0';
		
		unsigned int guidLen = strlen(player.GetGuid());
		char* guid = new char[guidLen + 1];
		memcpy(guid, player.GetGuid(), guidLen);
		guid[guidLen] = '\0';
	
		char* cmd = new char[9 + 1];
		strcpy(cmd, "JOIN");
		IPCCoD4Event* ipcEvent = new IPCCoD4Event(cmd);
		ipcEvent->AddArgument((void*) assignedSlot, IPCTypes::uint);
		ipcEvent->AddArgument((void*) ipAddress, IPCTypes::ch);
		ipcEvent->AddArgument((void*) guid, IPCTypes::ch);
		ipcEvent->AddArgument((void*) playerName, IPCTypes::ch);
		
		IPCServer::SetEventForBroadcast(ipcEvent);
	}
	
	return rtn;
}

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

int Events::HServerStatusRequest()
{
	char* cmd = new char[9 + 1];
	strcpy(cmd, "STATUSREQ");
	IPCCoD4Event* ipcEvent = new IPCCoD4Event(cmd);
	
	// Broadcast the event
	IPCServer::SetEventForBroadcast(ipcEvent);
	
	return 0;
}