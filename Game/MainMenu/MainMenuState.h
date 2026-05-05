#ifndef BVI_MAINMENUSTATE_H
#define BVI_MAINMENUSTATE_H

#include <Minigin/Core/IAppState.h>

namespace bvi::main_menu
{
	class MainMenuState final : public dae::core::IAppState
	{
	public:
		MainMenuState();
		~MainMenuState() override;

		MainMenuState( const MainMenuState& ) = delete;
		MainMenuState( MainMenuState&& ) = delete;
		MainMenuState& operator=( const MainMenuState& ) = delete;
		MainMenuState& operator=( MainMenuState&& ) = delete;

		void OnEnter() override;
		void OnExit() override;
		void Update( float /*deltaTime*/ ) override;
	};
}
#endif
