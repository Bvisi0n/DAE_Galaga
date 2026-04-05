#include <algorithm>

#include "Minigin/Core/SDBMHash.h"
#include "Minigin/Events/GameEvent.h"
#include "Minigin/Events/IObserver.h"
#include "Minigin/Events/ObservableComponent.h"

namespace dae::events
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
		// Prevents iterator invalidation if an observer detaches itself during notification.
		auto observersCopy = m_pObservers;
		for (auto observer : observersCopy)
		{
			observer->OnNotify(event);
		}
	}
}
