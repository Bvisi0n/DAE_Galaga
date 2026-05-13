#include <cassert>
#include <memory>
#include <utility>

#include <Minigin/Core/AppStateManager.h>
#include <Minigin/Core/IAppState.h>

namespace dae::core
{
	AppStateManager::AppStateManager() = default;
	AppStateManager::~AppStateManager() = default;

	void AppStateManager::ChangeState( std::unique_ptr<IAppState>&& newState )
	{
		assert( newState != nullptr && "newState payload cannot be null! Use ClearState() for teardown." );
		m_nextState = std::move( newState );
	}

	void AppStateManager::ClearState()
	{
		m_nextState.reset();
		m_currentState.reset();
	}

	void AppStateManager::Update( float deltaTime )
	{
		ProcessStateChange();
		if ( m_currentState != nullptr )
		{
			m_currentState->Update( deltaTime );
		}
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
}
