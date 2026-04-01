#include <algorithm>

#include "Components/ObservableComponent.h"
#include "ObserverPattern/GameEvent.h"
#include "ObserverPattern/IObserver.h"
#include "SDBMHash.h"

namespace dae
{
    void ObservableComponent::AttachObserver(IObserver* observer)
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

    void ObservableComponent::DetachObserver(IObserver * observer)
    {
        std::erase(m_pObservers, observer);
    }

    void ObservableComponent::NotifyObservers(GameEvent event) const
    {
        for (auto observer : m_pObservers)
        {
            observer->OnNotify(event);
        }
    }
}
