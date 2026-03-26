#include "Components/ScoreComponent.h"

namespace dae
{
    ScoreComponent::ScoreComponent(GameObject* pOwner)
        : ObservableComponent(pOwner)
        , m_score(0) {}

    void ScoreComponent::AddScore(int score)
    {
        m_score += score;
        NotifyObservers();
    }

    int ScoreComponent::GetScore() const
    {
        return m_score;
    }

}