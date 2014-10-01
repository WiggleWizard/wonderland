#ifndef IPC_RETURN_FUNCTION_H
#define	IPC_RETURN_FUNCTION_H

#include <vector>
#include <stdint.h>

class IPCReturnFunction {
public:
	IPCReturnFunction();
	IPCReturnFunction(const IPCReturnFunction& orig);
	virtual ~IPCReturnFunction();
	
	/**
	 * Parses the input packet into args and function request private vars.
	 * 
     * @param packet
	 * @param destructive If true, will consume and delete the packet.
     */
	void Parse(char* payload, bool destructive);
	
	/**
	 * Executes the function and puts the return into public variables.
     */
	void Execute();
	
	void* functionReturnPtr;
	uint32_t functionReturnType;
private:
	char* functionName;
	std::vector<void*> argv;
	std::vector<unsigned int> argt;
	
	uint32_t packetID;
};

#endif	/* IPC_RETURN_FUNCTION_H */

