#ifndef LIMBO_H
#define	LIMBO_H

class Limbo {
public:	
	Limbo()
	{
		this->state      = 0;
		this->ip         = NULL;
		this->denyReason = NULL;
	}
	
	virtual ~Limbo()
	{
		delete [] this->ip;
		delete [] this->denyReason;
	}
	
	unsigned int state;
	char* ip;
	unsigned int qPort;
	char* denyReason;
};

#endif	/* LIMBO_H */

