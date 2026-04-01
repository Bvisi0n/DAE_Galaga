#ifndef SCORECOMMAND_H
#define SCORECOMMAND_H

#include "Commands/Command.h"
#include "Components/ScoreComponent.h"

namespace dae
{
    class ScoreCommand final : public Command
    {
    public:
        ScoreCommand(ScoreComponent* scoreComponent, int valueOfChange)
            : m_pScoreComponent(scoreComponent)
            , m_valueOfChange(valueOfChange) {}

        void Execute(const float) override
        {
            m_pScoreComponent->AddScore(m_valueOfChange);
        }

    private:
        ScoreComponent* m_pScoreComponent;
        int m_valueOfChange{};
    };
}
#endif
