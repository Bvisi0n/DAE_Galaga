#ifndef DAE_APPSTATEMANAGER_H
#define DAE_APPSTATEMANAGER_H

#include <memory>

#include "Minigin/Core/IGameState.h"
#include "Minigin/Utilities/Singleton.h"

namespace dae::core
{
	class AppStateManager final : public utils::Singleton<AppStateManager>
	{
	public:
		void ChangeState( std::unique_ptr<IGameState> newState );
		void ProcessStateChange();
		void Update( float deltaTime );

	private:
		std::unique_ptr<IGameState> m_currentState{ nullptr };
		std::unique_ptr<IGameState> m_nextState{ nullptr };
	};
}
#endif
