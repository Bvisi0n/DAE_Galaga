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
