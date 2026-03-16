#include "Components/HealthComponent.h"

namespace dae
{
    HealthComponent::HealthComponent(dae::GameObject* pOwner, int lives)
        : Component(pOwner)
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
            NotifyObservers(dae::GameEvent::PlayerDied, m_lives);
        }
    }
}
