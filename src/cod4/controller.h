/*
 * Handles all internal CoD4 server processing.
 */

#ifndef BASE_H
#define	BASE_H

#include <vector>
#include <string>

class Controller {
public:
	
	Controller();
	
/*===============================================================*\
 * FUNCTIONS
\*===============================================================*/
	
	/**
	 * Initializes the controller.
     * @return 
     */
	void Initialize();
	
/*===============================================================*\
 * QUEUES
\*===============================================================*/
	
	std::vector<std::string> queueRX; // Recv
	std::vector<std::string> queueTX; // Trans
	
private:

};

#endif	/* BASE_H */

