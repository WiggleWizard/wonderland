#ifndef GLOBALS_H
#define	GLOBALS_H

// Enables or disables debug mode.
extern bool debugMode;

extern struct types {
	unsigned int uint = 1; // Unsigned int
	unsigned int ch   = 2; // Characters
} types;

extern int IPC_VER;
extern char* BANNER;

#endif	/* GLOBALS_H */

