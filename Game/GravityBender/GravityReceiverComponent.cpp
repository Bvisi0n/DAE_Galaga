#include <cmath>

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/MoveComponent.h>

#include <Game/GravityBender/GravityReceiverComponent.h>
#include <Game/GravityBender/GravityRegistry.h>

namespace bvi::gravity_bender
{
	GravityReceiverComponent::GravityReceiverComponent( dae::core::GameObject* owner )
		: Component( owner )
	{}

	GravityReceiverComponent::~GravityReceiverComponent() = default;

	void GravityReceiverComponent::InitializeLinkage()
	{
		m_moveComponent = GetOwner()->GetComponent<dae::core::MoveComponent>();
	}

	void GravityReceiverComponent::InitializeState()
	{}

	void GravityReceiverComponent::Update( const float )
	{
		if ( !m_moveComponent )
		{
			return;
		}

		const glm::vec3 currentPos = GetOwner()->GetTransform().GetWorldPosition();
		glm::vec3 totalGravityForce{ 0.0f, 0.0f, 0.0f };

		constexpr float epsilonSq = 2500.0f;

		for ( const auto& node : GravityRegistry::GetActiveNodes() )
		{
			const glm::vec3 diff = node.position - currentPos;
			const float distanceSq = glm::dot( diff, diff );

			if ( distanceSq < node.radiusSquared )
			{
				const float smoothedDistSq = distanceSq + epsilonSq;

				const float invSmoothedDist = 1.0f / std::sqrt( smoothedDistSq );
				const float invDistCube = ( 1.0f / smoothedDistSq ) * invSmoothedDist;

				totalGravityForce += diff * ( node.strength * invDistCube );
			}
		}

		m_moveComponent->AddForce( totalGravityForce );
	}
}
