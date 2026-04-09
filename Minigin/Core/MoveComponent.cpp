#include <cassert>

#include <glm/ext/vector_float3.hpp>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/MoveComponent.h"
#include "Minigin/Core/Transform.h"

namespace dae::core
{
	MoveComponent::MoveComponent( GameObject* owner, const float maxSpeed, const bool resetDirectionPerFrame )
		: Component( owner )
		, m_maxSpeed( maxSpeed )
		, m_resetDirectionPerFrame( resetDirectionPerFrame )
	{}

	void MoveComponent::InitializeLinkage()
	{}

	void MoveComponent::InitializeState()
	{}

	void MoveComponent::Update( const float deltaTime )
	{
		constexpr float epsilon = 0.0001f;
		if ( glm::dot( m_accumulatedDirection, m_accumulatedDirection ) < epsilon )
		{
			return;
		}

		const glm::vec3 normalizedDirection = glm::normalize( m_accumulatedDirection );

		const glm::vec3 velocity = normalizedDirection * m_maxSpeed * deltaTime;

		auto& transform = GetOwner()->GetTransform();
		transform.SetLocalPosition( transform.GetLocalPosition() + velocity );

		if ( m_resetDirectionPerFrame )
		{
			m_accumulatedDirection = glm::vec3{ 0.0f, 0.0f, 0.0f };
		}
	}

	void MoveComponent::AddDirection( const glm::vec3& direction )
	{
		m_accumulatedDirection += direction;
	}

	void MoveComponent::SetSpeed( const float speed )
	{
		if ( speed <= 0 )
		{
			assert( false && "Speed must be positive." );
			return;
		}

		m_maxSpeed = speed;
	}

	float MoveComponent::GetSpeed() const
	{
		return m_maxSpeed;
	}
}
