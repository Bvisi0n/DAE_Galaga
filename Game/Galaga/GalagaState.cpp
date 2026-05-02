#include <glm/ext/vector_int2.hpp>

#include <Minigin/Graphics/Renderer.h>
#include <Minigin/Scene/SceneManager.h>

#include <Game/Galaga/GalagaAssembler.h>
#include <Game/Galaga/GalagaState.h>

namespace bvi::galaga
{
	void GalagaState::OnEnter()
	{
		constexpr glm::ivec2 resolution{ 224, 288 };
		using Renderer = dae::graphics::Renderer;
		Renderer::GetInstance().SetLogicalResolution( resolution, Renderer::PresentationMode::Letterbox );

		GalagaAssembler::Assemble( this );
	}

	void GalagaState::OnExit()
	{
		dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
	}

	void GalagaState::Update( float /*deltaTime*/ )
	{}
}