#pragma once

#include "fmt/format.h"

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
#include <exception>

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
