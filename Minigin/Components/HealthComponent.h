#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "Components/Component.h"
#include "ObserverPattern/Subject.h"

namespace dae
{
    class HealthComponent final : public dae::Component, public dae::Subject
    {
    public:
        HealthComponent(dae::GameObject* pOwner, int lives);

        void Update(float) override {}
        void Render() const override {}

        int GetLives() const;

        void Die();

    private:
        int m_lives;
    };
}

#endif
