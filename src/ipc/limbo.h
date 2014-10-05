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
	char* denyReason;
};

#endif	/* LIMBO_H */

