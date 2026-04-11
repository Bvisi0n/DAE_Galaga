#include <cassert>
#include <cmath>
#include <memory>
#include <numbers>
#include <random>
#include <utility>

#include <SDL3/SDL_rect.h>

#include <glm/ext/vector_float3.hpp>

#include "Game/Blueprints/GravityBenderBlueprints.h"
#include "Game/Components/GravityReceiverComponent.h"
#include "Game/Components/ScreenWrapComponent.h"
#include "Game/Components/SpawnerPortalComponent.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/MoveComponent.h"
#include "Minigin/Graphics/PrimitiveRenderComponent.h"
#include "Minigin/Graphics/TextureComponent.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::components
{
	SpawnerPortalComponent::SpawnerPortalComponent( dae::core::GameObject* owner, const bvi::blueprints::ZakoData& blueprint )
		:Component( owner )
		, m_blueprint( blueprint )
	{}

	void SpawnerPortalComponent::InitializeLinkage()
	{
		m_primitiveRenderer = GetOwner()->GetComponent<dae::graphics::PrimitiveRenderComponent>();
		if ( !m_primitiveRenderer )
		{
			assert( m_primitiveRenderer && "requires a PrimitiveRenderComponent on the same GameObject." );
		}

		const auto pos = GetOwner()->GetTransform().GetWorldPosition();
		if ( m_primitiveRenderer )
		{
			SDL_FRect bounds{ pos.x, pos.y, 32.0f, 32.0f };
			m_primitiveRenderer->SetBounds( bounds );
		}
	}

	void SpawnerPortalComponent::InitializeState()
	{
		SetRandomDirection();
		SetRandomPosition();
	}

	void SpawnerPortalComponent::Update( const float deltaTime )
	{
		m_timer += deltaTime;

		// TODO L: Use switch instead.

		if ( m_currentState == PortalState::Exhausted )
		{
			if ( m_timer >= m_CooldownDuration )
			{
				m_currentState = PortalState::Anticipation;
				m_timer = 0.0f;
				if ( m_primitiveRenderer )
				{
					m_primitiveRenderer->SetEnabled( true );
				}
			}
		}
		else if ( m_currentState == PortalState::Anticipation )
		{
			if ( m_timer >= m_anticipationDuration )
			{
				m_currentState = PortalState::Spawning;
				m_timer = 0.0f;
				if ( m_primitiveRenderer )
				{
					m_primitiveRenderer->SetEnabled( true );
				}
			}
		}
		else if ( m_currentState == PortalState::Spawning )
		{
			if ( m_timer >= m_blueprint.spawnDelay )
			{
				EmitUnit();
				m_timer -= m_blueprint.spawnDelay;

				if ( ++m_spawnedCount >= m_blueprint.spawnCount )
				{
					m_currentState = PortalState::Exhausted;
					SetRandomDirection();
					SetRandomPosition();
					m_timer = 0.0f;
					m_spawnedCount = 0;
					if ( m_primitiveRenderer )
					{
						m_primitiveRenderer->SetEnabled( false );
					}
				}
			}
		}
	}

	void SpawnerPortalComponent::EmitUnit()
	{
		auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();
		auto unit = std::make_unique<dae::core::GameObject>();

		unit->GetTransform().SetLocalPosition( GetOwner()->GetTransform().GetWorldPosition() );
		unit->AddComponent<dae::graphics::TextureComponent>()->SetTexture( m_blueprint.filename.c_str() );
		unit->AddComponent<components::ScreenWrapComponent>( 1024.f, 576.f );
		unit->AddComponent<components::GravityReceiverComponent>();
		unit->AddComponent<dae::core::MoveComponent>( m_blueprint.speed, false )->AddDirection( m_direction );

		scene.AddGameObject( std::move( unit ) );
	}

	void SpawnerPortalComponent::SetRandomDirection()
	{
		std::random_device rd;
		std::mt19937 gen( rd() );
		std::uniform_real_distribution<float> dist( 0.0f, 2.0f * std::numbers::pi_v<float> );

		const float angle = dist( gen );
		m_direction = glm::vec3{ std::cos( angle ), std::sin( angle ), 0.0f };
	}

	void SpawnerPortalComponent::SetRandomPosition()
	{
		// TODO L: Fetch viewport dimensions.

		constexpr float viewportWidth = 1024.f;
		constexpr float viewportHeight = 576.f;
		constexpr float margin = 50.f;

		const float minX = margin;
		const float maxX = viewportWidth - margin;
		const float minY = margin;
		const float maxY = viewportHeight - margin;

		std::random_device rd;
		std::mt19937 gen( rd() );
		std::uniform_real_distribution<float> distX( minX, maxX );
		std::uniform_real_distribution<float> distY( minY, maxY );

		const float x = distX( gen );
		const float y = distY( gen );

		GetOwner()->GetTransform().SetLocalPosition( glm::vec3{ x, y, 0.0f } );

		if ( m_primitiveRenderer )
		{
			SDL_FRect bounds{ x - 8.0f, y - 8.0f, 32.0f, 32.0f };
			m_primitiveRenderer->SetBounds( bounds );
		}
	}
}
