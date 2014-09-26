#ifndef GLOBALS_H
#define	GLOBALS_H

// Enables or disables debug mode.
extern bool DEBUG_MODE;

extern struct IPCTypes {
	static const unsigned char uint = 0x01; // Unsigned int
	static const unsigned char sint = 0x02; // Signed int
	static const unsigned char ch   = 0x03; // Characters
} IPCTypes;

extern unsigned int IPC_VER;
extern const char* WONDERLAND_VER;

extern char BANNER[1024];

#endif	/* GLOBALS_H */

