#ifndef DAE_SDBMHASH_H
#define DAE_SDBMHASH_H

#include <string_view>

// DAE/Prog4/observer_eventqueue.pdf: "Inspired by: Learn C++ For Game Development by Bruce Sutherland"

namespace dae::core
{
	consteval unsigned int MakeSdbmHash( std::string_view text ) noexcept
	{
		unsigned int hash = 0;
		for ( const char character : text )
		{
			hash = static_cast<unsigned int>( character ) + ( hash << 6 ) + ( hash << 16 ) - hash;
		}
		return hash;
	}
}
#endif
