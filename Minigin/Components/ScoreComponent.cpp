#include "Components/ScoreComponent.h"
#include "ObserverPattern/GameEvent.h"
#include "SDBMHash.h"

namespace dae
{
    ScoreComponent::ScoreComponent(GameObject* pOwner)
        : ObservableComponent(pOwner)
        , m_score(0) {}

    void ScoreComponent::AddScore(const int score)
    {
        m_score += score;

        dae::GameEvent event{ dae::make_sdbm_hash("ScoreChanged") };
        event.PushArg(m_score);

        NotifyObservers(event);
    }

    int ScoreComponent::GetScore() const
    {
        return m_score;
    }
}
