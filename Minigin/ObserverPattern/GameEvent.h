#ifndef GAMEEVENT_H
#define GAMEEVENT_H

namespace dae
{
    // TODO N: Use Hash function instead of enum.
    enum class GameEvent
    {
        PlayerDied,
        ScoreChanged
    };
}
#endif
