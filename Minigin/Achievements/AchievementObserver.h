#ifndef ACHIEVEMENTOBSERVER_H
#define ACHIEVEMENTOBSERVER_H

#if USE_STEAMWORKS && !__EMSCRIPTEN__
    #include <steam_api.h>
#endif

#include "ObserverPattern/Observer.h"

// DAEL: Split into .h & .cpp.
// DAEL: Pimpl idiom candidate?

namespace dae
{
    class AchievementObserver final : public Observer
    {
    public:
        AchievementObserver() = default;
        virtual ~AchievementObserver() override = default;

        void OnNotify(GameEvent event, int value)  override
        {
            if (event == GameEvent::ScoreChanged && value >= 500)
            {
                #if USE_STEAMWORKS && !__EMSCRIPTEN__
                if (SteamUserStats())
                {
                    bool isAchieved = false;
                    if (SteamUserStats()->GetAchievement("ACH_WIN_ONE_GAME", &isAchieved) && !isAchieved)
                    {
                        SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
                        SteamUserStats()->StoreStats();
                    }
                }
                #endif
            }
        }
    };
}
#endif
