#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "Components/ObservableComponent.h"

namespace dae
{
    class GameObject;

    class HealthComponent final : public ObservableComponent
    {
    public:
        HealthComponent(GameObject* pOwner, int lives);

        void Update(const float) override {}

        [[nodiscard]] int GetLives() const;

        void Die();

    private:
        int m_lives;
    };
}
#endif
