#ifndef DAE_APPSTATEMANAGER_H
#define DAE_APPSTATEMANAGER_H

#include <memory>

#include <Minigin/Utilities/Singleton.h>

namespace dae::core
{
	class IAppState;

	class AppStateManager final : public utils::Singleton<AppStateManager>
	{
	public:
		~AppStateManager();

		AppStateManager( const AppStateManager& ) = delete;
		AppStateManager( AppStateManager&& ) = delete;
		AppStateManager& operator=( const AppStateManager& ) = delete;
		AppStateManager& operator=( AppStateManager&& ) = delete;

		void ChangeState( std::unique_ptr<IAppState>&& newState );
		void ChangeState( std::nullptr_t ) = delete;
		void ClearState();
		void Update( float deltaTime );

	private:
		friend class utils::Singleton<AppStateManager>;

		std::unique_ptr<IAppState> m_currentState;
		std::unique_ptr<IAppState> m_nextState;

		AppStateManager();
		void ProcessStateChange();
	};
}
#endif
