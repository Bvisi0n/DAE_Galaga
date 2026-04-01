#include "Components/HealthComponent.h"
#include "ObserverPattern/GameEvent.h"
#include "SDBMHash.h"

namespace dae
{
    HealthComponent::HealthComponent(GameObject* pOwner, int lives)
        : ObservableComponent(pOwner)
        , m_lives(lives) {}

    int HealthComponent::GetLives() const
    {
        return m_lives;
    }

    void HealthComponent::Die()
    {
        if (m_lives > 0)
        {
            --m_lives;

            dae::GameEvent event{ dae::make_sdbm_hash("PlayerDied") };
            event.PushArg(m_lives);

            NotifyObservers(event);
        }
    }
}
