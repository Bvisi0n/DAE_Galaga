#include <glm/ext/vector_int2.hpp>

#include <Minigin/Graphics/Renderer.h>
#include <Minigin/Scene/SceneManager.h>

#include <Game/GravityBender/GravityBenderAssembler.h>
#include <Game/GravityBender/GravityBenderState.h>
#include <Game/GravityBender/GravityRegistry.h>

namespace bvi::gravity_bender
{
	GravityBenderState::GravityBenderState() = default;
	GravityBenderState::~GravityBenderState() = default;

	void GravityBenderState::OnEnter()
	{
		constexpr glm::ivec2 resolution{ 1024, 576 };
		using Renderer = dae::graphics::Renderer;
		Renderer::GetInstance().SetLogicalResolution( resolution, Renderer::PresentationMode::Letterbox );

		GravityBenderAssembler::Assemble( this );
	}

	void GravityBenderState::OnExit()
	{
		dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
	}

	void GravityBenderState::Update( float deltaTime )
	{
		GravityRegistry::Update( deltaTime );
	}
}
