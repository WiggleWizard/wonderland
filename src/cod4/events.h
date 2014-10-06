#ifndef HOOK_EVENT_H
#define	HOOK_EVENT_H

#include <vector>
#include <cstdint>

class Hook;

class Events {
	static const unsigned long locPlayerJoinRequest = 0x0817153E;
	static const unsigned long locPlayerDisconnect  = 0x08170A26;
	static const unsigned long locPlayerSay         = 0x080AE962;
	static const unsigned long locPlayerNameChange  = 0x081705EC;
	static const unsigned long locRconStatus        = 0x0816C708;
	
	static const unsigned long locfuncIsPlayerConnectedAtSlot = 0x0813C0C4;
	static const unsigned long locfuncPacketResponseLoc       = 0x0813D086;
	static const unsigned long locfuncServerCommonInitLoc     = 0x080B563A; // 0x08123080
	
	// Function definitions, in case the events need to be recalled
	typedef int  (*funcdefServerCommonInit)(int32_t a1, uint32_t a2, int32_t a3, int32_t a4);
	typedef bool (*funcdefPlayerJoinRequest)(uint32_t a1, uint32_t ip, uint32_t a3, uint32_t a4, unsigned long a5);
	typedef int  (*funcdefPlayerDisconnect)(unsigned long playerOffset, void* a2, unsigned int reason);
	typedef bool (*funcdefIsPlayerConnectedAtSlot)(int a1, int a2, int a3, int a4, int a5, uint32_t offset1PtrVal, uint32_t offset2PtrVal, uint32_t offset3PtrVal);
	typedef bool (*funcdefPacketResponse)(signed int a1, unsigned long a2, uint32_t ip, unsigned long qPort, unsigned long a5, unsigned long a6, const char *a7);
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
	
	static int  HServerCommonInit(int32_t a1, uint32_t a2, int32_t a3, int32_t a4);
	static bool HPlayerJoinRequest(unsigned long a1, uint32_t ip, unsigned long a3, unsigned long a4, unsigned long a5);
	static int  HPlayerDisconnect(unsigned long playerOffset, void* a2, unsigned int reason);
	static int  HPlayerSay(unsigned int* playerId, int a2, int teamSay, char* message);
	static int  HPlayerNameChange(unsigned int playerOffset);
	static int  HServerStatusRequest();
	
/*===============================================================*\
 * HOOK VARIABLES
\*===============================================================*/
	
	static Hook* hServerCommonInit;
	static Hook* hPlayerJoinRequest;
	static Hook* hPlayerDisconnect;
	static Hook* hPlayerSay;
	static Hook* hPlayerChangeName;
	static Hook* hServerStatusRequest;
	
private:
	static std::vector<unsigned long> eventIds;
};

#endif	/* HOOK_EVENT_H */

