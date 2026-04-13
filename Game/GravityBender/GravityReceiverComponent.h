#ifndef GRAVITYRECEIVERCOMPONENT_H
#define GRAVITYRECEIVERCOMPONENT_H

#include <cmath>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "Game/GravityBender/GravityRegistry.h"

#include <Minigin/Core/GameObject.h>
#include "Minigin/Core/Component.h"
#include "Minigin/Core/MoveComponent.h"

namespace bvi::gravity_bender
{
	class GravityReceiverComponent final : public dae::core::Component
	{
	public:
		explicit GravityReceiverComponent( dae::core::GameObject* owner )
			: Component( owner )
		{}

		~GravityReceiverComponent() = default;
		GravityReceiverComponent( const GravityReceiverComponent& ) = delete;
		GravityReceiverComponent( GravityReceiverComponent&& ) = delete;
		GravityReceiverComponent& operator=( const GravityReceiverComponent& ) = delete;
		GravityReceiverComponent& operator=( GravityReceiverComponent&& ) = delete;

		void InitializeLinkage() override
		{
			m_moveComponent = GetOwner()->GetComponent<dae::core::MoveComponent>();
		}

		void InitializeState() override
		{}

		void Update( const float ) override
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

	private:
		dae::core::MoveComponent* m_moveComponent{ nullptr };
	};
}
#endif
