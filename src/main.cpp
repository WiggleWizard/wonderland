#include "ipc/ipc_server_man.h"
#include "cod4/events.h"

// When the application starts, ensure that the -sid arg is passed along with an integer
// this int is used in determining the base server ID for retrieving configs, executing ASes and other things
void  __attribute__((constructor)) init (int argc, char** argv)
{
	// Init the server manager
	IPCServer* serverMan = new IPCServer("Test");
	
	// Init the hooks into the server
	Events::InsertHooks();
}