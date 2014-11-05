/**
 * Author:  Terence-Lee 'Zinglish' Davis
 * License: MIT
 * Email:   zinglish[at]gmail.com
 * Twitter: @zingatsu
 */



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

unsigned int Player::GetIPAdr() {
	return *(uint32_t*) _GetPointerToData(mClientOffsets.net2);
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
	// Basically, we have an issue when forcing a name (this is mostly a hack, to make things less complicated
	// we modify the player's userinfo packet basically simulating as if a user actually changed
	// their name) the password the user is using is changed, if the server catches this it kicks them. So
	// we attempting to change the player's packet buffer without fucking the buffer up.
	
	// We convert to std::string to make things easier on ourselves
	std::string tmp((char*) this->offset + 1616);
	unsigned int pos = tmp.find("\\name\\");
	if(pos == string::npos)
		return;
	
	// Strip the current name declaration from the buffer
	tmp.erase(pos, 6 + strlen(this->GetName()));
	
	// Create the name structure again and push it into the buffer
	std::string a("\\name\\");
	std::string pName(name);
	tmp.insert(pos, a + pName);
	
	strncpy((char*)(this->offset + 1616), tmp.c_str(), tmp.length());
	*(char*)(this->offset + 1616 + tmp.length()) = 0x00; // C style string ending

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