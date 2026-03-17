#ifndef ACHIEVEMENTMANAGER_H
#define ACHIEVEMENTMANAGER_H

#include <memory>
#include <vector>

#include "AchievementObserver.h"
#include "Singletons/Singleton.h"

// If this ever gets fleshed out, consider https://gameprogrammingpatterns.com/service-locator.html before going all in on Singleton. Also consider swapping Observer out for Event-queue, they aren't time sensitive.

// DAEL: Split into .h & .cpp.

namespace dae
{
    class AchievementManager final : public Singleton<AchievementManager>
    {
    public:
        void Init()
        {
            m_pObserver = std::make_unique<AchievementObserver>();
        }

        AchievementObserver* GetAchievementObserver()
        {
            return m_pObserver.get();
        }

    private:
        friend class Singleton<AchievementManager>;

        AchievementManager() = default;

        std::unique_ptr<AchievementObserver> m_pObserver;
    };
}
#endif
