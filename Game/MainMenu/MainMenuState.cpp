#include <glm/ext/vector_int2.hpp>

#include <Minigin/Graphics/Renderer.h>
#include <Minigin/Scene/SceneManager.h>

#include <Game/MainMenu/MainMenuAssembler.h>
#include <Game/MainMenu/MainMenuState.h>

namespace bvi::main_menu
{
	MainMenuState::MainMenuState() = default;
	MainMenuState::~MainMenuState() = default;

	void MainMenuState::OnEnter()
	{
		constexpr glm::ivec2 resolution{ 1024, 576 };
		using Renderer = dae::graphics::Renderer;
		Renderer::GetInstance().SetLogicalResolution( resolution, Renderer::PresentationMode::Letterbox );

		MainMenuAssembler::Assemble( this );
	}

	void MainMenuState::OnExit()
	{
		dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
	}

	void MainMenuState::Update( const float /*deltaTime*/ )
	{}
}
