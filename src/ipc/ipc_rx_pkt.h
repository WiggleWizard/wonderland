/* 
 * File:   ipc_rx_packet.h
 * Author: zinglish
 *
 * Created on 21 May 2014, 20:34
 */

#ifndef IPC_RX_PACKET_H
#define	IPC_RX_PACKET_H

class IPCRxPacket {
public:
	IPCRxPacket();
	IPCRxPacket(const IPCRxPacket& orig);
	virtual ~IPCRxPacket();
private:
	void*        function; // Pointer to the function that's meant to exec
	unsigned int packetId;
};

#endif	/* IPC_RX_PACKET_H */

