#ifndef DAE_APPSTATEMANAGER_H
#define DAE_APPSTATEMANAGER_H

#include <memory>

#include "Minigin/Core/IAppState.h"
#include "Minigin/Utilities/Singleton.h"

namespace dae::core
{
	class AppStateManager final : public utils::Singleton<AppStateManager>
	{
	public:
		void ChangeState( std::unique_ptr<IAppState> newState );
		void ProcessStateChange();
		void Update( float deltaTime );

	private:
		std::unique_ptr<IAppState> m_currentState{ nullptr };
		std::unique_ptr<IAppState> m_nextState{ nullptr };
	};
}
#endif
