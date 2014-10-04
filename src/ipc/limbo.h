/* 
 * File:   limbo.h
 * Author: zinglish
 *
 * Created on 05 October 2014, 00:33
 */

#ifndef LIMBO_H
#define	LIMBO_H

class Limbo {
public:
	unsigned int state = 0;
	char* ip           = NULL;
	char* denyReason   = NULL;
	
	virtual ~Limbo()
	{
		delete [] this->ip;
		delete [] this->denyReason;
	}
};

#endif	/* LIMBO_H */

