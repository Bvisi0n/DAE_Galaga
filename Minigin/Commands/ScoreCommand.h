#ifndef SCORECOMMAND_H
#define SCORECOMMAND_H

#include "Commands/Command.h"
#include "Components/ScoreComponent.h"

// TODO L: Split into .h & .cpp.
// TODO N: Should trigger an event (Picked up pellet, killed an enemy), not add score. The observer should add score.

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
