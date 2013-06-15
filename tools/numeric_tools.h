#ifndef NUMERIC_TOOLS_H
#define NUMERIC_TOOLS_H

#include <type_traits>

namespace tools { namespace numeric {
	template <class T>
	auto as_signed (T && t)
	{
		return typename std::make_signed<T>::type(std::forward<T>(t));
	}
	
	template <class T>
	auto as_unsigned (T && t)
	{
		return typename std::make_unsigned<T>::type(std::forward<T>(t));
	}
	
} }


#endif
