#include <limits>
#include <memory>

#include <Minigin/Core/AppStateManager.h>
#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>

#include <Game/GravityBender/GravityBenderBlueprints.h>
#include <Game/GravityBender/GravityRegistry.h>
#include <Game/GravityBender/PlayerComponent.h>
#include <Game/MainMenu/MainMenuState.h>

namespace bvi::gravity_bender
{
	PlayerComponent::PlayerComponent( dae::core::GameObject* owner )
		: dae::core::Component( owner )
	{}

	PlayerComponent::~PlayerComponent()
	{
		GravityRegistry::RemovePlayerNode();

		using AppStateManager = dae::core::AppStateManager;
		using MainMenuState = main_menu::MainMenuState;
		AppStateManager::GetInstance().ChangeState( std::make_unique<MainMenuState>() );
	}

	void PlayerComponent::InitializeLinkage()
	{}

	void PlayerComponent::InitializeState()
	{}

	void PlayerComponent::Update( const float /*deltaTime*/ )
	{
		constexpr const auto& playerConfig = bvi::gravity_bender::config::Config.player;

		constexpr float radiusSquared{ playerConfig.gravityRadius * playerConfig.gravityRadius };

		const auto& origin = GetOwner()->GetTransform().GetWorldPosition();

		const GravityNode activeNode
		{
			.position = origin,
			.strength = playerConfig.gravityStrength,
			.radiusSquared = radiusSquared,
			.lifeTimeRemaining = std::numeric_limits<float>::infinity()
		};

		GravityRegistry::SetPlayerNode( activeNode );
	}
}
