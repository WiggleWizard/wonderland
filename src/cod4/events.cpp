#include "events.h"

#include "hook.h"
#include "callables.h"
#include "../globals.h"
#include "../ipc/ipc_event.h"
#include "../ipc/ipc_server_man.h"
#include "player.h"
#include "../ipc/limbo.h"

#include <arpa/inet.h>

Hook* Events::hServerCommonInit;
Hook* Events::hPlayerJoinRequest;
Hook* Events::hPlayerDisconnect;
Hook* Events::hPlayerSay;
Hook* Events::hPlayerChangeName;
Hook* Events::hServerStatusRequest;

Events::Events() {}
Events::Events(const Events& orig) {}
Events::~Events() {}

void Events::InsertHooks()
{
	Events::hServerCommonInit    = new Hook((void*) Events::locfuncServerCommonInitLoc, 5, (void*) Events::HServerCommonInit);
	Events::hPlayerJoinRequest   = new Hook((void*) Events::locPlayerJoinRequest, 5, (void*) Events::HPlayerJoinRequest);
	Events::hPlayerDisconnect    = new Hook((void*) Events::locPlayerDisconnect, 5, (void*) Events::HPlayerDisconnect);
	Events::hPlayerSay           = new Hook((void*) Events::locPlayerSay, 5, (void*) Events::HPlayerSay);
	Events::hPlayerChangeName    = new Hook((void*) Events::locPlayerNameChange, 5, (void*) Events::HPlayerNameChange);
	Events::hServerStatusRequest = new Hook((void*) Events::locRconStatus, 5, (void*) Events::HServerStatusRequest);
}

/*===============================================================*\
 * EVENTS
\*===============================================================*/

int Events::HServerCommonInit(int32_t a1, uint32_t a2, int32_t a3, int32_t a4)
{
	Events::hServerCommonInit->UnHook();
	int rtn = ((Events::funcdefServerCommonInit)Events::locfuncServerCommonInitLoc)(a1, a2, a3, a4);
	Events::hServerCommonInit->Rehook();
	
	// Mark the initialized state of the server
	IPCServer::Initialized();
	
	char* cmd = new char[4 + 1];
	strcpy(cmd, "INIT");
	IPCCoD4Event* ipcEvent = new IPCCoD4Event(cmd);
	
	IPCServer::SetEventForBroadcast(ipcEvent);
	
	return rtn;
}

bool Events::HPlayerJoinRequest(unsigned long a1, uint32_t ip, unsigned long qPort, unsigned long a4, unsigned long a5)
{
	// Predict slot ID
	unsigned int  slotID       = 0;
	unsigned long slotOffset   = 0x090B4F8C;
	
	unsigned int maxClients = Callables::GetMaxClients();
	for(uint32_t i = 0x090B4F8C; ;i += 677436)
	{
		if(slotID > maxClients)
			break;
		
		// If the slot's disconnected
		if(*(uint32_t*) i == 0)
		{
			slotOffset = i;
			slotID = 35580271 * ((i - 151736204) >> 2);
			break;
		}
	}
	
	printf("Slot %i predicted to connect\n", slotID);
	
	// Get the string value out of the IP
	// NOTE: Throughout this code you will see this IP being copied to memory.
	//       this is because this char* will be destroyed once in_addr goes out
	//       of scope.
	struct in_addr ip_addr;
	ip_addr.s_addr = ip;
	char* addr = inet_ntoa(ip_addr);
	
	// Iterate over the limbos and deal with each one according to what state it's
	// in. If there is no limbo then it will create one for that IP.
	bool hasLimbo          = false;
	unsigned int openLimbo = 0; // Limbo index free to overwrite
	bool foundOpenLimbo    = false;
	unsigned int s         = IPCServer::limbo.size();
	for(unsigned int i = 0; i < s; i++) 
	{
		Limbo* limbo = IPCServer::limbo[i];
		
		if(!limbo)
		{
			// Mark the index where we can insert a limbo if the IP does not have one
			if(!foundOpenLimbo)
			{
				foundOpenLimbo = true;
				openLimbo = i;
			}
			
			continue;
		}
		
		// We get to this part only if there's a limbo present at the current index
		if(strcmp(addr, limbo->ip) == 0)
		{
			// Mark the IP
			hasLimbo = true;
			
			// Limbo set to accept
			if(limbo->state == 1)
			{
				// Execute the origional request to join the server
				Events::hPlayerJoinRequest->UnHook();
				bool rtn = ((Events::funcdefPlayerJoinRequest)Events::locPlayerJoinRequest)(a1, ip, qPort, a4, a5);
				Events::hPlayerJoinRequest->Rehook();
				
				// After the request is executed, we will check if the connection was a success
				// then fire another event.
				Player player(slotID);
				printf("Slot %i conn state: %i, name: %s\n", slotID, player.GetConnState(), player.GetName());
				if(player.GetConnState() > 0)
				{
					// Copy IP into memory
					char* ipAddress = new char[strlen(addr) + 1];
					memcpy(ipAddress, addr, strlen(addr));
					ipAddress[strlen(addr)] = '\0';

					// Player name to memory
					unsigned int playerNameLen = strlen(player.GetName());
					char* playerName = new char[playerNameLen + 1];
					memcpy(playerName, player.GetName(), playerNameLen);
					playerName[playerNameLen] = '\0';

					// GUID to memory
					unsigned int guidLen = strlen(player.GetGuid());
					char* guid = new char[guidLen + 1];
					memcpy(guid, player.GetGuid(), guidLen);
					guid[guidLen] = '\0';

					char* cmd = new char[4 + 1];
					strcpy(cmd, "JOIN");
					IPCCoD4Event* ipcEvent = new IPCCoD4Event(cmd);
					ipcEvent->AddArgument((void*) slotID, IPCTypes::uint);
					ipcEvent->AddArgument((void*) ipAddress, IPCTypes::ch);
					ipcEvent->AddArgument((void*) guid, IPCTypes::ch);
					ipcEvent->AddArgument((void*) playerName, IPCTypes::ch);

					// Broadcast created event
					IPCServer::SetEventForBroadcast(ipcEvent);
					printf("JOIN event fired\n");
				}
				
				// Destroy limbo
				delete limbo;
				IPCServer::limbo[i] = NULL;
			}
			// Limbo is set to deny
			else if(limbo->state == 2)
			{
				((funcdefPacketResponse)locfuncPacketResponseLoc)(1, a1, ip, qPort, 0x000000, 0x000000, limbo->denyReason);
				
				// Destroy limbo
				delete limbo;
				IPCServer::limbo[i] = NULL;
			}
			
			break;
		}
	}
	
	// No limbo match was made, so we make one here and fire the request
	// event.
	if(!hasLimbo)
	{
		printf("Limbo does not exist, making one now\n");
		
		Limbo* limbo = new Limbo();

		// Copy IP into memory
		char* ipAddressLimbo = new char[strlen(addr) + 1];
		memcpy(ipAddressLimbo, addr, strlen(addr));
		ipAddressLimbo[strlen(addr)] = '\0';
		limbo->ip = ipAddressLimbo;

		// Insert into limbo queue
		IPCServer::limbo.at(openLimbo) = limbo;

		// Copy IP address for JOINREQ event
		char* ipAddress = new char[strlen(addr) + 1];
		memcpy(ipAddress, addr, strlen(addr));
		ipAddress[strlen(addr)] = '\0';

		char* cmd = new char[7 + 1];
		strcpy(cmd, "JOINREQ");
		IPCCoD4Event* ipcEvent = new IPCCoD4Event(cmd);
		ipcEvent->AddArgument((void*) ipAddress, IPCTypes::ch);

		// Broadcast the event
		IPCServer::SetEventForBroadcast(ipcEvent);
	}
}

int Events::HPlayerDisconnect(unsigned long playerOffset, void* a2, unsigned int reason)
{
	unsigned int slotID = 35580271 * ((playerOffset - 151736204) >> 2);
	
	char* cmd = new char[10 + 1];
	strcpy(cmd, "DISCONNECT");
	IPCCoD4Event* ipcEvent = new IPCCoD4Event(cmd);
	ipcEvent->AddArgument((void*) slotID, IPCTypes::uint);
	
	IPCServer::SetEventForBroadcast(ipcEvent);
	
	// Call the original function
	Events::hPlayerDisconnect->UnHook();
	int rtn = ((Events::funcdefPlayerDisconnect)Events::locPlayerDisconnect)(playerOffset, a2, reason);
	Events::hPlayerDisconnect->Rehook();
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
	if(message[0] == 0x15)
	{
		message += 1;
	}
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