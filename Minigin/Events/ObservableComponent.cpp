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
		// std::ranges::find not supported by local emscripten. (works on web...?)
		auto it = std::find( m_observers.begin(), m_observers.end(), observer );
		if ( it != m_observers.end() )
		{
			if ( m_isNotifying )
			{
				*it = nullptr;
			}
			else
			{
				m_observers.erase( it );
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
