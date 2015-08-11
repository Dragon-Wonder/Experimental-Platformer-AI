#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/**********************************************************************************************************************************************/
#include "main.h"
#include <time.h>
#include <cmath>
/**********************************************************************************************************************************************/
class Tick {
	private: 
		//Members
		uchar fps;
	
		//Functions
		void sleep(ulong);
		
	public:
		//Default Constructor
		Tick();
		~Tick();
		
		//Members
		uint Clock;
		
		//Functions
		void wait(void); //Call this to activate the wait
	
};
/**********************************************************************************************************************************************/
#endif
