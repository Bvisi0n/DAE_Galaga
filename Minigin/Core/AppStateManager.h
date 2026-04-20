#ifndef APPSTATEMANAGER_H
#define APPSTATEMANAGER_H

#include <memory>
#include <utility>

#include "Minigin/Core/IGameState.h"
#include "Minigin/Utilities/Singleton.h"

namespace dae::core
{
	class AppStateManager final : public utils::Singleton<AppStateManager>
	{
	public:
		void PushState( std::unique_ptr<IGameState> newState )
		{
			m_nextState = std::move( newState );
		}

		void ProcessStateChange()
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

		void Update( float deltaTime )
		{
			if ( m_currentState != nullptr )
			{
				m_currentState->Update( deltaTime );
			}
		}

	private:
		std::unique_ptr<IGameState> m_currentState{ nullptr };
		std::unique_ptr<IGameState> m_nextState{ nullptr };
	};
}
#endif
