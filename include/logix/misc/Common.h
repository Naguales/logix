#pragma once

#include "fmt/format.h"
#include "logix/misc/scope_guard.hpp"

#ifdef _WIN32
	#include <windows.h>
	
	#ifdef max
		#undef max
	#endif
	#ifdef min
		#undef min
	#endif
#endif // Windows

#include <string>
#include <vector>
#include <chrono>
#include <exception>
#include <numeric>

#define _USE_MATH_DEFINES
#include <math.h>

#define __FILENAME__ std::max<const char*>(__FILE__,\
    std::max(strrchr(__FILE__, '\\')+1, strrchr(__FILE__, '/')+1))

namespace logix
{

	/**
	 * An auxiliary class for conveniently assigning the description
	 * of 'what' occurred when representing an exception.
	 */
	class LogixException : public std::exception
	{
	public:
		explicit LogixException(std::string s)
			: m_error(std::move(s)) {}
		explicit LogixException(char const* s)
			: m_error(s) {}

		inline char const* what() const noexcept override { return m_error.data(); }

	private:
		std::string m_error;
	};

} // namespace logix
