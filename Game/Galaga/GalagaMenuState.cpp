#include <glm/ext/vector_int2.hpp>

#include <Minigin/Graphics/Renderer.h>
#include <Minigin/Scene/SceneManager.h>

#include <Game/Galaga/GalagaMenuAssembler.h>
#include <Game/Galaga/GalagaMenuState.h>

namespace bvi::galaga
{
	void GalagaMenuState::OnEnter()
	{
		constexpr glm::ivec2 resolution{ 224, 288 };
		using Renderer = dae::graphics::Renderer;
		Renderer::GetInstance().SetLogicalResolution( resolution, Renderer::PresentationMode::Letterbox );

		GalagaMenuAssembler::Assemble( this );
	}

	void GalagaMenuState::OnExit()
	{
		dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
	}

	void GalagaMenuState::Update( float /*deltaTime*/ )
	{}
}