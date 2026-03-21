#include <algorithm>

#include "Components/ScoreComponent.h"
#include "ObserverPattern/GameEvent.h"
#include "ObserverPattern/IObserver.h"
#include "SDBMHash.h"

namespace dae
{
    ScoreComponent::ScoreComponent(GameObject* pOwner)
        : Component(pOwner)
        , m_score(0) {}

    void ScoreComponent::AttachObserver(IObserver* observer)
    {
        if (observer == nullptr)
        {
            return;
        }
        else if (std::ranges::find(m_pObservers, observer) == m_pObservers.end())
        {
            m_pObservers.push_back(observer);
        }
    }

    void ScoreComponent::DetachObserver(IObserver* observer)
    {
        std::erase(m_pObservers, observer);
    }

    void ScoreComponent::NotifyObservers()
    {
        GameEvent event(make_sdbm_hash("ScoreChanged"));
        for (auto observer : m_pObservers)
        {
            observer->OnNotify(event);
        }
    }

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