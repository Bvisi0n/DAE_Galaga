#ifndef BVI_MAINMENUSTATE_H
#define BVI_MAINMENUSTATE_H

#include <glm/ext/vector_int2.hpp>

#include <Minigin/Core/IAppState.h>
#include <Minigin/Graphics/Renderer.h>
#include <Minigin/Scene/SceneManager.h>

#include <Game/GravityBender/GravityBenderState.h>
#include <Game/MainMenu/MainMenuAssembler.h>

// TODO bvi_main_menu - Split into .cpp/.h

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
			constexpr glm::ivec2 resolution{ 1024, 576 };
			using Renderer = dae::graphics::Renderer;
			Renderer::GetInstance().SetLogicalResolution( resolution, Renderer::PresentationMode::Letterbox );

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
