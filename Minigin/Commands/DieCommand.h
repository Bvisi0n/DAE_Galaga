#ifndef DIECOMMAND_H
#define DIECOMMAND_H

#include "Commands/Command.h"
#include "Components/HealthComponent.h"

namespace dae
{
    class DieCommand final : public Command
    {
    public:
        DieCommand(HealthComponent* healthComponent)
            : m_pHealthComponent(healthComponent) {}

        void Execute(const float) override
        {
            m_pHealthComponent->Die();
        }

    private:
        HealthComponent* m_pHealthComponent;
    };
}
#endif
