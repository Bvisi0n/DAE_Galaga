#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

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
			MainMenuBuilder::Build( scene );

		#ifdef ENABLE_GRAVITY_BENDER
			auto pushStateCommand = std::make_unique<common::PushStateCommand<gravity_bender::GravityBenderState>>( this );
			m_bindings.emplace_back( dae::input::Keyboard::Key::F, dae::input::InputManager::KeyState::Down, std::move( pushStateCommand ) );
		#endif
		}

		void OnExit() override
		{
			dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
			dae::input::InputManager::GetInstance().ClearAllBinds(); // Safety measure.
		}

		void Update( float /*deltaTime*/ ) override
		{}

	private:
		std::vector<dae::input::ScopedInputBinding> m_bindings{};
	};
}
#endif
