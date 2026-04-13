#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "Game/Common/PushStateCommand.h"
#include "Game/GravityBender/GravityBenderState.h"
#include "Game/MainMenu/MainMenuBuilder.h"

#include "Minigin/Core/IGameState.h"
#include "Minigin/Input/InputManager.h"
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
			MainMenuBuilder::Build( scene );

			//TODO L: ENABLE_GRAVITY_BENDER doesn't work as it should.
		#ifdef ENABLE_GRAVITY_BENDER
			dae::input::InputManager::GetInstance().BindCommand( dae::input::Keyboard::Key::F, dae::input::InputManager::KeyState::Down, std::make_unique<common::PushStateCommand<gravity_bender::GravityBenderState>>( this ) );
		#endif
		}

		void OnExit() override
		{
			dae::scenes::SceneManager::GetInstance().RemoveAllScenes();

		#ifdef ENABLE_GRAVITY_BENDER
			dae::input::InputManager::GetInstance().UnbindCommand( dae::input::Keyboard::Key::F, dae::input::InputManager::KeyState::Down );
		#endif
		}

		void Update() override
		{}
	};
}
#endif
