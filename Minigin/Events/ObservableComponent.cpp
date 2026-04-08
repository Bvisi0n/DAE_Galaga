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
			m_pPendingObservers.push_back( observer );
		}
		else
		{
			m_pObservers.push_back( observer );
		}
	}

	void ObservableComponent::DetachObserver( IObserver* observer )
	{
		// std::ranges::find not supported by local emscripten. (works on web...?)
		auto it = std::find( m_pObservers.begin(), m_pObservers.end(), observer );
		if ( it != m_pObservers.end() )
		{
			if ( m_isNotifying )
			{
				*it = nullptr;
			}
			else
			{
				m_pObservers.erase( it );
			}
		}
	}

	void ObservableComponent::NotifyObservers( GameEvent event )
	{
		m_isNotifying = true;

		for ( IObserver* observer : m_pObservers )
		{
			if ( observer != nullptr )
			{
				observer->OnNotify( event );
			}
		}

		m_isNotifying = false;

		std::erase( m_pObservers, nullptr );

		if ( !m_pPendingObservers.empty() )
		{
			m_pObservers.insert( m_pObservers.end(), m_pPendingObservers.begin(), m_pPendingObservers.end() );
			m_pPendingObservers.clear();
		}
	}
}
