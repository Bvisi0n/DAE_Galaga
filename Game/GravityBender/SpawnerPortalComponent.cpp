#include <cassert>
#include <cmath>
#include <memory>
#include <numbers>
#include <random>
#include <utility>

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

#include <Minigin/Core/ColliderComponent.h>
#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/MoveComponent.h>
#include <Minigin/Graphics/PrimitiveRenderComponent.h>
#include <Minigin/Scene/Scene.h>
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
		SpawnIndicator();
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
					m_blueprint.cooldownSeconds *= 0.9F;

					if ( m_primitiveRenderer != nullptr )
					{
						m_primitiveRenderer->SetEnabled( true );
					}
					SpawnIndicator();
				}
				break;
			}
			case PortalState::Anticipation:
			{
				if ( m_timer >= m_blueprint.anticipationSeconds )
				{
					m_currentState = PortalState::Spawning;
					m_timer = 0.F;
					DestroyIndicator();
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

		const SDL_FColor enemyColor
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

		using MoveComponent = dae::core::MoveComponent;
		using MoveDescriptor = dae::core::MoveDescriptor;
		unit->AddComponent<MoveComponent>
			(
				MoveDescriptor
				{
					.maxSpeed = m_blueprint.unitSpeed
				}
			)->SetVelocity( m_direction );

		using ColliderComponent = dae::core::ColliderComponent;
		using Size2D = dae::core::Size2D;
		unit->AddComponent<ColliderComponent>
			(
				Size2D
				{
					.width = m_blueprint.unitSize,
					.height = m_blueprint.unitSize
				},
				1
			);

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
		const float viewportWidth = config::c_GlobalConfig.viewport.width;
		const float viewportHeight = config::c_GlobalConfig.viewport.height;

		static std::random_device randomDevice;
		static std::mt19937 generator( randomDevice() );
		std::uniform_real_distribution<float> xDistribution( margin, viewportWidth - margin );
		std::uniform_real_distribution<float> yDistribution( margin, viewportHeight - margin );

		GetOwner()->GetTransform().SetLocalPosition( glm::vec3{ xDistribution( generator ), yDistribution( generator ), 0.F } );
	}

	void SpawnerPortalComponent::SpawnIndicator()
	{
		if ( m_indicator != nullptr )
		{
			return;
		}

		auto& scene{ dae::scenes::SceneManager::GetInstance().GetActiveScene() };
		auto indicator{ std::make_unique<dae::core::GameObject>() };

		const dae::graphics::CircleShape circleConfig
		{
			.radius = 5.0F,
			.isFilled = true
		};

		const SDL_FColor orange{ 1.0F, 0.65F, 0.0F, 1.0F };
		indicator->AddComponent<dae::graphics::PrimitiveRenderComponent>
			(
				dae::graphics::PrimitiveShape{ circleConfig },
				orange
			);

		const float speed = glm::length( m_direction );
		const glm::vec3 normalizedDir = ( speed > 0.001F ) ? ( m_direction / speed ) : glm::vec3{ 0.F, 1.F, 0.F };

		constexpr float visualPadding = 15.0F;
		const float totalOffset = ( m_blueprint.unitSize / 2.0F ) + visualPadding;
		indicator->GetTransform().SetLocalPosition( normalizedDir * totalOffset );

		m_indicator = indicator.get();
		indicator->SetParent( GetOwner(), false );
		scene.AddGameObject( std::move( indicator ) );
	}

	void SpawnerPortalComponent::DestroyIndicator()
	{
		if ( m_indicator != nullptr )
		{
			m_indicator->MarkForDeletion();
			m_indicator = nullptr;
		}
	}
}
