#ifndef GLOBALS_H
#define	GLOBALS_H

// Enables or disables debug mode.
extern bool DEBUG_MODE;

extern struct IPCTypes {
	unsigned int uint = 1; // Unsigned int
	unsigned int sint = 2; // Signed int
	unsigned int ch   = 3; // Characters
} ipcTypes;

extern unsigned int IPC_VER;
extern const char* WONDERLAND_VER;

extern char BANNER[1024];

#endif	/* GLOBALS_H */

