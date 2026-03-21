#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <vector>

#include "Components/Component.h"
#include "ObserverPattern/ISubject.h"

namespace dae
{
    class GameObject;

    class HealthComponent final : public Component, public ISubject
    {
    public:
        HealthComponent(GameObject* pOwner, int lives);

        void Update(float) override {}
        void Render() const override {}

        void AttachObserver(IObserver* observer) override;
        void DetachObserver(IObserver* observer) override;
        void NotifyObservers() override;

        int GetLives() const;

        void Die();

    private:
        std::vector<IObserver*> m_pObservers;
        int m_lives;
    };
}

#endif
