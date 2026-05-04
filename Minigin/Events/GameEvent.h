#ifndef DAE_GAMEEVENT_H
#define DAE_GAMEEVENT_H

#include <array>
#include <cassert>
#include <cstdint>
#include <utility>
#include <variant>

// Inspired by DAE/Prog4/observer_eventqueue.pdf

// TODO dae_input - Split into .cpp/.h

namespace dae::events
{
	struct Vector2D
	{
		float x;
		float y;
	};

	using EventId = uint32_t;
	using EntityId = uint32_t;
	using EventArg = std::variant<int32_t, float, bool, Vector2D, EntityId>;

	struct GameEvent
	{
		EventId id;
		static constexpr uint8_t c_MaxArgs{ 4 };
		uint8_t argumentCount{ 0 };
		std::array<EventArg, c_MaxArgs> args{};

		explicit GameEvent( EventId eventId ) noexcept
			: id{ eventId }
		{}

		template <typename T>
		void PushArg( T&& argument )
		{
			if ( argumentCount >= c_MaxArgs )
			{
				assert( false && "GameEvent: Maximum arguments exceeded." );
				return;
			}

			args.at( argumentCount++ ) = std::forward<T>( argument );
		}
	};
}

#endif
