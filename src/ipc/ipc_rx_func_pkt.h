/**
 * Author:  Terence-Lee 'Zinglish' Davis
 * License: MIT
 * Email:   zinglish[at]gmail.com
 * Twitter: @zingatsu
 */

/* 
 * Recieved function calls
 */

#ifndef IPC_RX_PACKET_H
#define	IPC_RX_PACKET_H

class IPCRxFuncPacket {
public:
	IPCRxFuncPacket();
	IPCRxFuncPacket(const IPCRxFuncPacket& orig);
	virtual ~IPCRxFuncPacket();
private:
	void*        function; // Pointer to the function that's meant to exec
	unsigned int packetId;
};

#endif	/* IPC_RX_PACKET_H */

