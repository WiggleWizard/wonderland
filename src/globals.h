#ifndef GLOBALS_H
#define	GLOBALS_H

// Enables or disables debug mode.
extern bool DEBUG_MODE;

extern struct IPCTypes {
	static const unsigned int uint = 1; // Unsigned int
	static const unsigned int sint = 2; // Signed int
	static const unsigned int ch   = 3; // Characters
} IPCTypes;

extern unsigned int IPC_VER;
extern const char* WONDERLAND_VER;

extern char BANNER[1024];

#endif	/* GLOBALS_H */

