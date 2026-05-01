#include <algorithm>
#include <cmath>

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/MoveComponent.h>
#include <Minigin/Core/Transform.h>

namespace dae::core
{
	MoveComponent::MoveComponent( GameObject* owner, const MoveDescriptor& descriptor )
		: Component( owner )
	{
		SetDragCoefficient( descriptor.drag );
		SetMaxSpeed( descriptor.maxSpeed );
	}

	void MoveComponent::InitializeLinkage()
	{}

	void MoveComponent::InitializeState()
	{}

	void MoveComponent::Update( const float deltaTime )
	{
		m_velocity += m_accumulatedForces * deltaTime;
		m_accumulatedForces = { 0.F, 0.F, 0.F };

		const float dragMultiplier = std::max( 0.F, 1.F - ( m_dragCoefficient * deltaTime ) );
		m_velocity *= dragMultiplier;

		const float currentSpeedSq = glm::dot( m_velocity, m_velocity );
		constexpr float epsilon = 0.0001F;

		if ( currentSpeedSq > epsilon )
		{
			if ( currentSpeedSq > m_maxSpeedSq )
			{
				const float currentSpeed = std::sqrt( currentSpeedSq );
				const float maxSpeed = std::sqrt( m_maxSpeedSq );
				m_velocity = ( m_velocity / currentSpeed ) * maxSpeed;
			}

			auto& transform = GetOwner()->GetTransform();
			transform.SetLocalPosition( transform.GetLocalPosition() + ( m_velocity * deltaTime ) );
		}
		else
		{
			m_velocity = { 0.F, 0.F, 0.F };
		}
	}

	void MoveComponent::AddForce( const glm::vec3& force )
	{
		m_accumulatedForces += force;
	}

	void MoveComponent::SetDragCoefficient( const float drag )
	{
		m_dragCoefficient = std::max( 0.F, drag );
	}

	float MoveComponent::GetDragCoefficient() const noexcept
	{
		return m_dragCoefficient;
	}

	void MoveComponent::SetMaxSpeed( const float speed )
	{
		m_maxSpeedSq = speed * speed;
	}

	void MoveComponent::SetVelocity( const glm::vec3& velocity )
	{
		m_velocity = velocity;
	}

	const glm::vec3& MoveComponent::GetVelocity() const
	{
		return m_velocity;
	}
}
