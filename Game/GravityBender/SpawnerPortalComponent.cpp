#include <cassert>
#include <cmath>
#include <memory>
#include <numbers>
#include <random>
#include <utility>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

#include <glm/ext/vector_float3.hpp>

#include <Minigin/Core/ColliderComponent.h>
#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/MoveComponent.h>
#include <Minigin/Graphics/PrimitiveRenderComponent.h>
#include <Minigin/Scene/SceneManager.h>

#include <Game/GravityBender/GravityBenderBlueprints.h>
#include <Game/GravityBender/GravityReceiverComponent.h>
#include <Game/GravityBender/ScreenWrapComponent.h>
#include <Game/GravityBender/SpawnerPortalComponent.h>

namespace bvi::gravity_bender
{
	SpawnerPortalComponent::SpawnerPortalComponent( dae::core::GameObject* owner, const UnitData& blueprint )
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
	}

	void SpawnerPortalComponent::InitializeState()
	{
		SetRandomDirection();
		SetRandomPosition();
	}

	void SpawnerPortalComponent::Update( const float deltaTime )
	{
		m_timer += deltaTime;

		switch ( m_currentState )
		{
			case PortalState::Exhausted:
			{
				if ( m_timer >= m_CooldownDuration )
				{
					m_currentState = PortalState::Anticipation;
					m_timer = 0.0f;
					if ( m_primitiveRenderer ) m_primitiveRenderer->SetEnabled( true );
				}
				break;
			}
			case PortalState::Anticipation:
			{
				if ( m_timer >= m_anticipationDuration )
				{
					m_currentState = PortalState::Spawning;
					m_timer = 0.0f;
				}
				break;
			}
			case PortalState::Spawning:
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
						if ( m_primitiveRenderer ) m_primitiveRenderer->SetEnabled( false );
					}
				}
				break;
			}
		}
	}

	void SpawnerPortalComponent::EmitUnit()
	{
		auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();
		auto unit = std::make_unique<dae::core::GameObject>();

		unit->GetTransform().SetLocalPosition( GetOwner()->GetTransform().GetWorldPosition() );
		unit->AddComponent<dae::graphics::PrimitiveRenderComponent>(
	dae::graphics::PrimitiveShape{ dae::graphics::RectShape{ SDL_FRect{ -2.5f, -2.5f, 5.f, 5.f }, true } }, SDL_Color{ 255, 50, 50, 255 } );

		unit->AddComponent<dae::core::MoveComponent>( m_blueprint.speed )->SetVelocity( m_direction );
		unit->AddComponent<dae::core::ColliderComponent>( 5.f, 5.f, 1 );
		unit->AddComponent<ScreenWrapComponent>( 1024.f, 576.f );
		unit->AddComponent<GravityReceiverComponent>();

		scene.AddGameObject( std::move( unit ) );
	}

	void SpawnerPortalComponent::SetRandomDirection()
	{
		static std::random_device rd;
		static std::mt19937 gen( rd() );
		std::uniform_real_distribution<float> dist( 0.0f, 2.0f * std::numbers::pi_v<float> );

		const float angle = dist( gen );
		constexpr float maxSpeed = 200.0f;
		m_direction = glm::vec3{ std::cos( angle ), std::sin( angle ), 0.0f } * maxSpeed;
	}

	void SpawnerPortalComponent::SetRandomPosition()
	{
		constexpr float viewportWidth = 1024.f;
		constexpr float viewportHeight = 576.f;
		constexpr float margin = 50.f;

		static std::random_device rd;
		static std::mt19937 gen( rd() );
		std::uniform_real_distribution<float> distX( margin, viewportWidth - margin );
		std::uniform_real_distribution<float> distY( margin, viewportHeight - margin );

		GetOwner()->GetTransform().SetLocalPosition( glm::vec3{ distX( gen ), distY( gen ), 0.0f } );
	}
}
