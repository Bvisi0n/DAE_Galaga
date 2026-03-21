#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <cstdint>

// DAE/Prog4/observer_eventqueue.pdf

// Example usage:
//  GameEvent event(make_sdbm_hash("PlayerDied"));
//  if(event.id == make_sdbm_hash("PlayerDied"))
//  {
//      handle player death event
//  }

namespace dae
{
    struct EventArg {};

    using EventId = unsigned int;

    struct GameEvent
    {
        const EventId id;
        static const uint8_t MAX_ARGS = 8;
        uint8_t nbArgs{};
        EventArg args[MAX_ARGS]{};

        explicit GameEvent(EventId _id)
            : id{ _id } {}
    };
}
#endif
