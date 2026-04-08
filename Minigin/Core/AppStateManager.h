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
			m_NextState = std::move( newState );
		}

		void ProcessStateChange()
		{
			if ( m_NextState != nullptr )
			{
				if ( m_CurrentState != nullptr )
				{
					m_CurrentState->OnExit();
				}

				m_CurrentState = std::move( m_NextState );
				m_CurrentState->OnEnter();
			}
		}

		void Update()
		{
			if ( m_CurrentState != nullptr )
			{
				m_CurrentState->Update();
			}
		}

	private:
		std::unique_ptr<IGameState> m_CurrentState{ nullptr };
		std::unique_ptr<IGameState> m_NextState{ nullptr };
	};
}
#endif
