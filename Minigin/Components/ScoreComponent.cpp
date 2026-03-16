#include "Components/ScoreComponent.h"

namespace dae
{
    ScoreComponent::ScoreComponent(GameObject* pOwner)
        : Component(pOwner)
        , m_score(0) {}

    void ScoreComponent::AddScore(int score)
    {
        m_score += score;
        NotifyObservers(GameEvent::ScoreChanged, m_score);
    }

    int ScoreComponent::GetScore() const
    {
        return m_score;
    }

}