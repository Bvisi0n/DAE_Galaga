#include <algorithm>
#include <vector>

#include "Minigin/Events/GameEvent.h"
#include "Minigin/Events/IObserver.h"
#include "Minigin/Events/ObservableComponent.h"

namespace dae::events
{
	void ObservableComponent::AttachObserver( IObserver* pObserver )
	{
		if ( m_isNotifying )
		{
			m_pPendingObservers.push_back( pObserver );
		}
		else
		{
			m_pObservers.push_back( pObserver );
		}
	}

	void ObservableComponent::DetachObserver( IObserver* pObserver )
	{
		auto it = std::ranges::find( m_pObservers, pObserver );
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

		for ( IObserver* p_observer : m_pObservers )
		{
			if ( p_observer != nullptr )
			{
				p_observer->OnNotify( event );
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
