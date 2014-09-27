

#include "player.h"

#include <string>
#include <cstring>

using std::string;

// Initialize the player
Player::Player(unsigned int playerId)
{
    this->id     = playerId;
	this->offset = Player::smClientsLoc + Player::smClientSize * this->id;
}

bool Player::IsInGame() {
    return (GetConnState() == 4);
}

char* Player::GetName() {
    return (char*) _GetPointerToData(mClientOffsets.name);
}

char* Player::GetGuid() {
    return (char*) _GetPointerToData(mClientOffsets.guid);
}

char* Player::GetIPAdr() {
    unsigned long* net1 = (unsigned long*) _GetPointerToData(mClientOffsets.net1);
    void* net2 = _GetDataAsPointer(mClientOffsets.net2);
    unsigned long* net3 = (unsigned long*) _GetPointerToData(mClientOffsets.net3);
    unsigned long* net4 = (unsigned long*) _GetPointerToData(mClientOffsets.net4);
    void* net5 = _GetDataAsPointer(mClientOffsets.net5);
	
    //return utils::strxpld((char*) ((AdrToStringFuncType) smAdrToStringFunc)(*net1, net2, *net3, *net4, net5), ":", 1).at(0);
	return NULL;
}

char* Player::GetChanBuffer() {
	return (char*)((Player::smClientsLoc + Player::smClientSize * this->id) + Player::mClientOffsets.chanBuffer);
}

/**
 * Gets the player's connection state
 * @return Pointer to connection state
 */
int Player::GetConnState() {
	return *(int*) this->_GetPointerToData(mClientOffsets.state);
}

/**
 * Kicks a player using a client command, if the player has a hack to block the 'w' command then it won't kick them
 * this needs to be patched!
 */
void Player::Kick(char* reason) {
    if (GetConnState() > 0)
        ((mClientCmdFuncType) smSendClientCmdFunc)((void*) (smClientsLoc + smClientSize * this->id), NULL, "%s \"%s\"", "w", reason);
}

void Player::SetName(char* name)
{
	strncpy((char*)(this->offset + 1616), "name\\", 5);
	strncpy((char*)(this->offset + 1616 + 5), name, 15);

	((mPlayerForceUserinfo)smPlayerForceUserinfoLoc)(this->offset);
	((mPlayerForceName)smPlayerForceNameLoc)(this->id);
}

void Player::SendMessage(char* message)
{
	if(GetConnState() > 0)
        ((mClientCmdFuncType) smSendClientCmdFunc)((void*) (smClientsLoc + smClientSize * this->id), NULL, "%s \"%s\"", "h", message);
}

/**
 * 
 * P R I V A T E  F U N C T I O N S
 * 
 */
void* Player::_GetDataAsPointer(const unsigned long offset) {
    return (void*) *((unsigned long*) ((Player::smClientsLoc + Player::smClientSize * this->id) + offset));
}

void* Player::_GetPointerToData(const unsigned long offset) {
    return (void*) ((Player::smClientsLoc + Player::smClientSize * this->id) + offset);
}

Player::~Player()
{
    
}