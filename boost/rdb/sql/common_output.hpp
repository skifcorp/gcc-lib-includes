#ifndef __COMMON_OUTPUT_
#define __COMMON_OUTPUT_

#include <sstream>

template<class T>
void str(std::ostream& os, const T& obj) {
  obj.str(os);
}
	 
#endif
