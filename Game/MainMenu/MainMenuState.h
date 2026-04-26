#ifndef BVI_MAINMENUSTATE_H
#define BVI_MAINMENUSTATE_H

#include "Game/GravityBender/GravityBenderState.h"
#include "Game/MainMenu/MainMenuAssembler.h"

#include "Minigin/Core/IAppState.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::main_menu
{
	class MainMenuState final : public dae::core::IAppState
	{
	public:
		MainMenuState() = default;
		~MainMenuState() override = default;

		MainMenuState( const MainMenuState& ) = delete;
		MainMenuState( MainMenuState&& ) = delete;
		MainMenuState& operator=( const MainMenuState& ) = delete;
		MainMenuState& operator=( MainMenuState&& ) = delete;

		void OnEnter() override
		{
			MainMenuAssembler::Assemble( this );
		}

		void OnExit() override
		{
			dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
		}

		void Update( float /*deltaTime*/ ) override
		{}
	};
}
#endif
