#include <algorithm>
#include <cmath>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/MoveComponent.h"
#include "Minigin/Core/Transform.h"

namespace dae::core
{
	MoveComponent::MoveComponent( GameObject* owner, const float maxSpeed, const float drag )
		: Component( owner )
	{
		SetDragCoefficient( drag );
		SetMaxSpeed( maxSpeed );
	}

	void MoveComponent::InitializeLinkage()
	{}

	void MoveComponent::InitializeState()
	{}

	void MoveComponent::Update( const float deltaTime )
	{
		m_velocity += m_accumulatedForces * deltaTime;
		m_accumulatedForces = { 0.0f, 0.0f, 0.0f };

		const float dragMultiplier = std::max( 0.0f, 1.0f - ( m_dragCoefficient * deltaTime ) );
		m_velocity *= dragMultiplier;

		const float currentSpeedSq = glm::dot( m_velocity, m_velocity );
		constexpr float epsilon = 0.0001f;

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
			m_velocity = { 0.0f, 0.0f, 0.0f };
		}
	}

	void MoveComponent::AddForce( const glm::vec3& force )
	{
		m_accumulatedForces += force;
	}

	void MoveComponent::SetDragCoefficient( const float drag )
	{
		m_dragCoefficient = std::max( 0.0f, drag );
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
