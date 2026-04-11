#ifndef GRAVITYRECEIVERCOMPONENT_H
#define GRAVITYRECEIVERCOMPONENT_H

#include <cmath>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "Game/Physics/GravityRegistry.h"

#include <Minigin/Core/GameObject.h>
#include "Minigin/Core/Component.h"
#include "Minigin/Core/MoveComponent.h"

namespace bvi::components
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

			for ( const auto& node : physics::GravityRegistry::GetActiveNodes() )
			{
				const glm::vec3 diff = node.position - currentPos;
				const float distanceSq = glm::dot( diff, diff );

				if ( distanceSq < node.radiusSquared && distanceSq > 0.001f )
				{
					const float distance = std::sqrt( distanceSq );
					const glm::vec3 direction = diff / distance;

					totalGravityForce += direction * ( node.strength / distanceSq );
				}
			}

			m_moveComponent->AddForce( totalGravityForce );
		}

	private:
		dae::core::MoveComponent* m_moveComponent{ nullptr };
	};
}
#endif
