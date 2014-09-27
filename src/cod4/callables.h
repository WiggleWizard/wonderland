#ifndef HOOK_CALLABLE_H
#define	HOOK_CALLABLE_H

#include <stdint.h>

class Callables {
public:
	Callables();
	Callables(const Callables& orig);
	virtual ~Callables();
	
	uint32_t GetPlayerIdByOffset(uint32_t offset);
private:

};

#endif	/* HOOK_CALLABLE_H */

