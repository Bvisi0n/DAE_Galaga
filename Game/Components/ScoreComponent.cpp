#include "Game/Components/ScoreComponent.h"

#include "Minigin/Core/SDBMHash.h"
#include "Minigin/Events/GameEvent.h"

namespace bvi
{
    ScoreComponent::ScoreComponent(dae::core::GameObject* pOwner)
        : ObservableComponent(pOwner)
        , m_score(0) {}

    void ScoreComponent::InitializeState() noexcept
    {
        NotifyScoreChanged();
    }

    void ScoreComponent::AddScore(const int score)
    {
        m_score += score;
        NotifyScoreChanged();
    }

    int ScoreComponent::GetScore() const
    {
        return m_score;
    }

    void ScoreComponent::NotifyScoreChanged() const
    {
        dae::events::GameEvent event{ dae::core::make_sdbm_hash("ScoreChanged") };
        event.PushArg(m_score);
        NotifyObservers(event);
    }
}
