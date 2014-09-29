#include "callables.h"

#include "player.h"

Callables::Callables() {
}

Callables::Callables(const Callables& orig) {
}

Callables::~Callables() {
}

uint32_t Callables::GetPlayerIdByOffset(uint32_t offset)
{
	return (offset - 0x090B4F8C) / 677436;
}

void Callables::BroadcastChat(char* message)
{
	uint32_t maxClients = Callables::GetMaxClients();
	
	for(unsigned int i = 0; i < maxClients; i++)
	{
		Player player(i);
		
		if(player.GetConnState() != 4)
			continue;

		player.SendMessage(message);
	}
}

uint32_t Callables::GetMaxClients()
{
	return *(uint32_t*) locMaxClients;
}

char* Callables::GetValueFromSlashString(char* slashedString, char* key)
{
	return ((funcdefGetValueFromSlashString)locfuncGetValueFromSlashString)(slashedString, key);
}