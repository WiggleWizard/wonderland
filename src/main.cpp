/**
 * Found some interesting stuff, @0816242B that runs GSC scripting
 */

#include <cstring>

#include "ipc/ipc_server_man.h"
#include "cod4/events.h"

#include "globals.h"

// When the application starts, ensure that the -sid arg is passed along with an integer
// this int is used in determining the base server ID for retrieving configs, executing ASes and other things
void  __attribute__((constructor)) init (int argc, char** argv)
{
	printf(BANNER);
	
	// Declare the Wonderland identifier for later use
	char* wid = NULL;
	
	// Get args from the command line
	for(int i = 0; i < argc; i++)
	{
		if(strcmp(argv[i], "-wid") == 0)
		{
			printf("Using Wonderland identifier: %s\n", argv[i + 1]);
			
			unsigned int argvLen = strlen(argv[i + 1]);
			wid = new char[argvLen];
			memcpy(wid, argv[i + 1], argvLen);
			wid[argvLen] = '\0';
		}
	}
	
	if(wid == NULL)
	{
		printf("You must specify a Wonderland identifier by using '-wid' as a switch in your CLI\n");
		exit(0);
	}
	
	// Init the server manager
	IPCServer* serverMan = new IPCServer(wid);
	
	// Init the hooks into the server
	Events::InsertHooks();
}