#include <algorithm>
#include <vector>

#include "Minigin/Events/GameEvent.h"
#include "Minigin/Events/IObserver.h"
#include "Minigin/Events/ObservableComponent.h"

namespace dae::events
{
	void ObservableComponent::AttachObserver( IObserver* observer )
	{
		if ( m_isNotifying )
		{
			m_pendingObservers.push_back( observer );
		}
		else
		{
			m_observers.push_back( observer );
		}
	}

	void ObservableComponent::DetachObserver( IObserver* observer )
	{
		auto iterator = std::ranges::find( m_observers, observer );
		if ( iterator != m_observers.end() )
		{
			if ( m_isNotifying )
			{
				*iterator = nullptr;
			}
			else
			{
				m_observers.erase( iterator );
			}
		}
	}

	void ObservableComponent::NotifyObservers( GameEvent event )
	{
		m_isNotifying = true;

		for ( IObserver* observer : m_observers )
		{
			if ( observer != nullptr )
			{
				observer->OnNotify( event );
			}
		}

		m_isNotifying = false;

		std::erase( m_observers, nullptr );

		if ( !m_pendingObservers.empty() )
		{
			m_observers.insert( m_observers.end(), m_pendingObservers.begin(), m_pendingObservers.end() );
			m_pendingObservers.clear();
		}
	}
}
