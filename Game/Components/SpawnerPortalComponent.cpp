#include <memory>
#include <utility>

#include <glm/ext/vector_float3.hpp>

#include "Game/Blueprints/GravityBenderBlueprints.h"
#include "Game/Components/SpawnerPortalComponent.h"
#include "Game/Components/ScreenWrapComponent.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/MoveComponent.h"
#include "Minigin/Graphics/TextureComponent.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::components
{
	SpawnerPortalComponent::SpawnerPortalComponent( dae::core::GameObject* owner, const bvi::blueprints::ZakoData& blueprint )
		:Component( owner )
		, m_Blueprint( blueprint )
	{}

	void SpawnerPortalComponent::InitializeLinkage()
	{}

	void SpawnerPortalComponent::InitializeState()
	{}

	void SpawnerPortalComponent::Update( const float deltaTime )
	{
		if ( m_CurrentState == PortalState::Exhausted )
		{
			// TODO H: Flag for delete.
			return;
		}

		m_Timer += deltaTime;

		if ( m_CurrentState == PortalState::Anticipation )
		{
			if ( m_Timer >= m_AnticipationDuration )
			{
				m_CurrentState = PortalState::Spawning;
				m_Timer = 0.0f;
			}
		}
		else if ( m_CurrentState == PortalState::Spawning )
		{
			if ( m_Timer >= m_Blueprint.spawnDelay )
			{
				EmitZako();
				m_Timer -= m_Blueprint.spawnDelay;

				if ( ++m_SpawnedCount >= m_Blueprint.spawnCount )
				{
					m_CurrentState = PortalState::Exhausted;
				}
			}
		}
	}

	void SpawnerPortalComponent::EmitZako()
	{
		auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();
		auto zako = std::make_unique<dae::core::GameObject>();

		zako->GetTransform().SetLocalPosition( GetOwner()->GetTransform().GetWorldPosition() );
		zako->AddComponent<dae::graphics::TextureComponent>()->SetTexture( m_Blueprint.filename.c_str() );
		zako->AddComponent<components::ScreenWrapComponent>( 1024.f, 576.f );
		zako->AddComponent<dae::core::MoveComponent>( 200.f, false )->AddDirection( glm::vec3{ 1.0f, 1.0f, 0.0f } );

		scene.AddGameObject( std::move( zako ) );
	}
}
