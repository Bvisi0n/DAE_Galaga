#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <cassert>
#include <cstdint>
#include <variant>

// Inspired by DAE/Prog4/observer_eventqueue.pdf

// Example usage:
//  GameEvent event(make_sdbm_hash("PlayerTakesDamage"));
//  event.PushArg(damageAmount);
// 
//  if(event.id == make_sdbm_hash("PlayerTakesDamage"))
//  {
//      float damageTaken = std::get<float>(event.args[0]);
//      handle event
//  }
// 
// Consider std::visit as alternative but more research is needed, probably with lambda's...

namespace dae::events
{
	struct Vector2D { float x, y; };
	using EventId = uint32_t;
	using EntityId = uint32_t;
	using EventArg = std::variant<int32_t, float, bool, Vector2D, EntityId>;

	struct GameEvent
	{
		const EventId id;

		// Should pack nicely within 64 Bytes
		static constexpr uint8_t MAX_ARGS{ 4 };
		
		uint8_t argumentCount{ 0 };
		EventArg args[MAX_ARGS]{};

		explicit GameEvent(EventId _id) noexcept
			: id{ _id } {}

		template <typename T>
		void PushArg(T&& argument)
		{
			if (argumentCount >= MAX_ARGS)
			{
				assert(false && "GameEvent: Maximum arguments exceeded.");
				return;
			}

			args[argumentCount++] = std::forward<T>(argument);
		}
	};
}
#endif
