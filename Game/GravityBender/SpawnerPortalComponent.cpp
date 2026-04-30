#include <cassert>
#include <cmath>
#include <memory>
#include <numbers>
#include <random>
#include <utility>

#include <glm/ext/vector_float3.hpp>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

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
	SpawnerPortalComponent::SpawnerPortalComponent( dae::core::GameObject* owner, config::PortalBlueprint blueprint )
		: Component( owner )
		, m_blueprint( blueprint )
	{}

	void SpawnerPortalComponent::InitializeLinkage()
	{
		m_primitiveRenderer = GetOwner()->GetComponent<dae::graphics::PrimitiveRenderComponent>();
		if ( m_primitiveRenderer == nullptr )
		{
			assert( m_primitiveRenderer && "requires a PrimitiveRenderComponent on the same GameObject." );
		}
	}

	void SpawnerPortalComponent::InitializeState()
	{
		SetRandomDirection();
		SetRandomPosition();
	}

	void SpawnerPortalComponent::Update( float deltaTime )
	{
		m_timer += deltaTime;

		switch ( m_currentState )
		{
			case PortalState::Exhausted:
			{
				if ( m_timer >= m_blueprint.cooldownSeconds )
				{
					m_currentState = PortalState::Anticipation;
					m_timer = 0.F;

					if ( m_primitiveRenderer != nullptr )
					{
						m_primitiveRenderer->SetEnabled( true );
					}
				}
				break;
			}
			case PortalState::Anticipation:
			{
				if ( m_timer >= m_blueprint.anticipationSeconds )
				{
					m_currentState = PortalState::Spawning;
					m_timer = 0.F;
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
						m_timer = 0.F;
						m_spawnedCount = 0;

						if ( m_primitiveRenderer != nullptr )
						{
							m_primitiveRenderer->SetEnabled( false );
						}
					}
				}
				break;
			}
		}
	}

	void SpawnerPortalComponent::EmitUnit()
	{
		const float halfSize{ m_blueprint.unitSize / 2.F };

		const SDL_FRect localBounds
		{
			.x = -halfSize,
			.y = -halfSize,
			.w = m_blueprint.unitSize,
			.h = m_blueprint.unitSize
		};

		const dae::graphics::RectShape rectConfig
		{
			.bounds = localBounds,
			.isFilled = true
		};

		const SDL_Color enemyColor
		{
			.r = m_blueprint.unitColor.r,
			.g = m_blueprint.unitColor.g,
			.b = m_blueprint.unitColor.b,
			.a = m_blueprint.unitColor.a
		};

		auto& scene{ dae::scenes::SceneManager::GetInstance().GetActiveScene() };
		auto unit{ std::make_unique<dae::core::GameObject>() };

		unit->GetTransform().SetLocalPosition( GetOwner()->GetTransform().GetWorldPosition() );

		unit->AddComponent<dae::graphics::PrimitiveRenderComponent>
			( dae::graphics::PrimitiveShape{ rectConfig }, enemyColor );

		unit->AddComponent<dae::core::MoveComponent>
			( m_blueprint.unitSpeed )->SetVelocity( m_direction );

		unit->AddComponent<dae::core::ColliderComponent>( 5.F, 5.F, 1 );
		unit->AddComponent<ScreenWrapComponent>( halfSize );
		unit->AddComponent<GravityReceiverComponent>();

		scene.AddGameObject( std::move( unit ) );
	}

	void SpawnerPortalComponent::SetRandomDirection()
	{
		static std::random_device randomDevice;
		static std::mt19937 generator( randomDevice() );
		std::uniform_real_distribution<float> distribution( 0.F, 2.F * std::numbers::pi_v<float> );

		const float angle = distribution( generator );

		m_direction = glm::vec3{ std::cos( angle ), std::sin( angle ), 0.F } * m_blueprint.unitSpeed;
	}

	void SpawnerPortalComponent::SetRandomPosition()
	{
		namespace config = bvi::gravity_bender::config;

		constexpr float margin = 50.F;
		const float viewportWidth = config::Config.viewport.width;
		const float viewportHeight = config::Config.viewport.height;

		static std::random_device randomDevice;
		static std::mt19937 generator( randomDevice() );
		std::uniform_real_distribution<float> xDistribution( margin, viewportWidth - margin );
		std::uniform_real_distribution<float> yDistribution( margin, viewportHeight - margin );

		GetOwner()->GetTransform().SetLocalPosition( glm::vec3{ xDistribution( generator ), yDistribution( generator ), 0.F } );
	}
}