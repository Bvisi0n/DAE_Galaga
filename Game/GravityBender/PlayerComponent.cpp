#include <cassert>
#include <limits>
#include <memory>

#include <glm/ext/vector_float3.hpp>

#include "Game/Common/MoveCommand.h"
#include "Game/GravityBender/GravityRegistry.h"
#include "Game/GravityBender/PlayerComponent.h"
#include "Game/GravityBender/SpawnGravityFieldCommand.h"
#include "Game/MainMenu/MainMenuState.h"

#include "Minigin/Core/AppStateManager.h"
#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/MoveComponent.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Input/Keyboard.h"

namespace bvi::gravity_bender
{
	PlayerComponent::PlayerComponent( dae::core::GameObject* owner )
		: dae::core::Component( owner )
	{}

	PlayerComponent::~PlayerComponent()
	{
		GravityRegistry::RemovePlayerNode();
		dae::core::AppStateManager::GetInstance().PushState( std::make_unique<bvi::main_menu::MainMenuState>() );
	}

	void PlayerComponent::InitializeLinkage()
	{
		auto* moveComp = GetOwner()->GetComponent<dae::core::MoveComponent>();

		if ( !moveComp )
		{
			assert( moveComp && "PlayerComponent requires a MoveComponent." );
			return;
		}

		using KeyState = dae::input::InputManager::KeyState;
		using Key = dae::input::Keyboard::Key;
		using MoveCommand = common::MoveCommand;
		using vec3 = glm::vec3;

		m_bindings.emplace_back( Key::W, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ 0.0f, -400.0f, 0.0f } ) );

		m_bindings.emplace_back( Key::S, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ 0.0f, 400.0f, 0.0f } ) );

		m_bindings.emplace_back( Key::A, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ -400.0f, 0.0f, 0.0f } ) );

		m_bindings.emplace_back( Key::D, KeyState::Pressed, std::make_unique<MoveCommand>( moveComp, vec3{ 400.0f, 0.0f, 0.0f } ) );

		m_bindings.emplace_back( Key::Space, KeyState::Pressed, std::make_unique<SpawnGravityFieldCommand>( GetOwner() ) );
	}

	void PlayerComponent::InitializeState()
	{}

	void PlayerComponent::Update( const float )
	{
		// TODO GRAV: For prototyping purposes, there are better ways to do this.
		const auto& origin = GetOwner()->GetTransform().GetWorldPosition();
		constexpr float radius = 100.0f;
		constexpr float strength = 1'500'000.0f;
		GravityRegistry::SetPlayerNode( GravityNode{ origin, strength, radius * radius, std::numeric_limits<float>::infinity() } );
	}
}
