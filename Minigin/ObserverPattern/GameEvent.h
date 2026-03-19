#ifndef GAMEEVENT_H
#define GAMEEVENT_H

namespace dae
{
    // DAEN: Use Hash function instead of enum.
    enum class GameEvent
    {
        PlayerDied,
        ScoreChanged
    };
}
#endif
