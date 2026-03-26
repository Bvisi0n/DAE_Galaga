#include "Components/HealthComponent.h"

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
            NotifyObservers();
        }
    }
}
