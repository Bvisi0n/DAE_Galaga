#include <memory>
#include <utility>

#include "Minigin/Core/AppStateManager.h"
#include "Minigin/Core/IAppState.h"

namespace dae::core
{
	void AppStateManager::ChangeState( std::unique_ptr<IAppState> newState )
	{
		m_nextState = std::move( newState );
	}

	void AppStateManager::ProcessStateChange()
	{
		if ( m_nextState != nullptr )
		{
			if ( m_currentState != nullptr )
			{
				m_currentState->OnExit();
			}

			m_currentState = std::move( m_nextState );
			m_currentState->OnEnter();
		}
	}

	void AppStateManager::Update( float deltaTime )
	{
		if ( m_currentState != nullptr )
		{
			m_currentState->Update( deltaTime );
		}
	}
}
