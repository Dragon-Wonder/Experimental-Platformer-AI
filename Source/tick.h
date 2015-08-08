#ifndef __TICK__HEADER__
#define __TICK__HEADER__
/**********************************************************************************************************************************************/
#include "main.h"
#include <time.h>
#include <cmath>
/**********************************************************************************************************************************************/
class Tick {
	private: 
		
		const uchar fps = 20;
	
		void sleep(ulong);
		
	public:
	
		void wait(void);
	
};
/**********************************************************************************************************************************************/
#endif
