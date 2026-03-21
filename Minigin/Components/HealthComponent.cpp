#include <algorithm>

#include "Components/HealthComponent.h"
#include "ObserverPattern/GameEvent.h"
#include "ObserverPattern/IObserver.h"
#include "SDBMHash.h"

namespace dae
{
    HealthComponent::HealthComponent(GameObject* pOwner, int lives)
        : Component(pOwner)
        , m_lives(lives) {}

    void HealthComponent::AttachObserver(IObserver* observer)
    {
        if (observer == nullptr)
        {
            return;
        }
        else if (std::ranges::find(m_pObservers, observer) == m_pObservers.end())
        {
            m_pObservers.push_back(observer);
        }
    }

    void HealthComponent::DetachObserver(IObserver * observer)
    {
        std::erase(m_pObservers, observer);
    }

    void HealthComponent::NotifyObservers()
    {
        GameEvent event(make_sdbm_hash("PlayerDied"));
        for (auto observer : m_pObservers)
        {
            observer->OnNotify(event);
        }
    }

    int HealthComponent::GetLives() const
    {
        return m_lives;
    }

    void HealthComponent::Die()
    {
        if (m_lives > 0)
        {
            --m_lives;
            NotifyObservers();
        }
    }
}
