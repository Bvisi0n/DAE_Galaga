#ifndef BVI_MAINMENUSTATE_H
#define BVI_MAINMENUSTATE_H

#include "Game/Common/PushStateCommand.h"
#include "Game/GravityBender/GravityBenderState.h"
#include "Game/MainMenu/MainMenuBuilder.h"

#include "Minigin/Core/IGameState.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Input/ScopedInputBinding.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::main_menu
{
	class MainMenuState final : public dae::core::IGameState
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
			auto& scene = dae::scenes::SceneManager::GetInstance().CreateScene();
			MainMenuBuilder::Build( scene, this );
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
