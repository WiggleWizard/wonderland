/**
 * Author:  Terence-Lee 'Zinglish' Davis
 * License: MIT
 * Email:   zinglish[at]gmail.com
 * Twitter: @zingatsu
 */

// Chan sets player name: (1228 * [player id] + 0x840CCD0)
// 0x84bd120 | Editable player
// 0x84bd13c | F32 | Player's X
// 0x84bd29c | Byte | Player's ammo count
// 0x84c0168 | String | Player's Name

#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <string>
#include <vector>

class Player {
	// Aliased types
	typedef const unsigned long (ulong);

	// Virtual variable locations
	static const unsigned long smClientsLoc    = 0x090B4F8C; // Start of client structures
	static const unsigned long smMaxClientsLoc = 0x08370624; // Max clients allowed on the server
	
	static const unsigned long smClientSize = 677436;

	// Virtual function locations
	static const unsigned long smSendClientCmdFunc      = 0x08177402; // Function pointer of sv_send_client_command (68h for chat?)
	static const unsigned long smAdrToStringFunc        = 0x0813BB30; // Function pointer of NET_AdrToString
	static const unsigned long smPlayerForceUserinfoLoc = 0x081703D6;
	static const unsigned long smPlayerForceNameLoc     = 0x080A8068;

	// Virtual types`
	typedef void*        (*AdrToStringFuncType)(unsigned long a1, void* a2, unsigned long a3, unsigned long a4, void* a5);
	typedef int          (*mClientCmdFuncType)(void* clientPtr, signed int a2, const char *a3, ...);
	typedef unsigned int (*mPlayerForceUserinfo)(unsigned long playerPointer);
	typedef unsigned int (*mPlayerForceName)(unsigned long playerId);


	public:
		// ctor
		Player(unsigned int playerId);
		void Purge();

		// Data return
		bool  IsInGame();
		char* GetName();
		char* GetGuid();
		unsigned int GetIPAdr();
		char* GetChanBuffer();
		int   GetConnState();

		// Commands
		void SendMessage(char* message);
		void Kick(char* reason);
		void SetName(char* name);

		// Variables
		unsigned int id;
		unsigned int offset;

		// Connection state of the client
		typedef enum {
			CS_FREE, // can be reused for a new connection
			CS_ZOMBIE, // client has been disconnected, but don't reuse connection for a couple seconds
			CS_CONNECTED, // has been assigned to a client_t, but no gamestate yet
			CS_PRIMED, // gamestate has been sent, but client hasn't sent a usercmd
			CS_ACTIVE // client is fully in game
		} mClientStateType;

		// All the offsets for each piece of data for a single client/player

		static struct mClientOffsetStruct {
			static ulong state = 0;
			static ulong net1 = 32;
			static ulong net2 = 36;
			static ulong net3 = 40;
			static ulong net4 = 44;
			static ulong net5 = 48;
			static ulong chanBuffer = 1616;
			static ulong name = 135824;
			static ulong guid = 658688;
		} mClientOffsets;

		// Holds read only data of each client (Writing to this space is useless)

		static struct mClientDataStruct {
			mClientStateType state; // 4 bytes
			static ulong net1; // All this net crap is used to generate an IP
			static ulong net2;
			static ulong net3;
			static ulong net4;
			static ulong net5;
			static char* name[15];
			static char* guid[32];
		} mClientData; // 677436 bytes in total
		
		virtual ~Player();
	private:
		void* _GetPointerToData(const unsigned long offset);
		void* _GetDataAsPointer(const unsigned long offset);
};

#endif // PLAYER_H