#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "Game/Builders/MenuBuilder.h"
#include "Game/Commands/PushStateCommand.h"
#include "Game/States/GravityBenderState.h"

#include "Minigin/Core/IGameState.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::states
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
			bvi::builders::MenuBuilder::Build( scene );
			dae::input::InputManager::GetInstance().BindCommand( dae::input::Keyboard::Key::F, dae::input::InputManager::KeyState::Down, std::make_unique<bvi::commands::PushStateCommand<GravityBenderState>>( this ) );
		}

		void OnExit() override
		{
			dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
			dae::input::InputManager::GetInstance().UnbindCommand( dae::input::Keyboard::Key::F, dae::input::InputManager::KeyState::Down );
		}

		void Update() override
		{}
	};
}
#endif
