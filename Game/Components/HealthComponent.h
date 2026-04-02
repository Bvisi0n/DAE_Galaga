#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "Minigin/Events/ObservableComponent.h"

namespace dae
{
    class core::GameObject;

    class HealthComponent final : public events::ObservableComponent
    {
    public:
        HealthComponent(core::GameObject* pOwner, int lives);

        void Initialize() override;
        void Update(const float) noexcept override {}

        [[nodiscard]] int GetLives() const;

        void Die();

    private:
        int m_lives;

        void NotifyPlayerDied() const;
    };
}
#endif
