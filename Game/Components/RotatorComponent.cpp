#include <cmath>
#include <numbers>

#include "Game/Components/RotatorComponent.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"

namespace bvi
{
	RotatorComponent::RotatorComponent( dae::core::GameObject* owner, float range, float speed )
		: Component( owner ), m_range( range ), m_rotationSpeed( speed )
	{}

	void RotatorComponent::Update( const float deltaTime )
	{
		m_angle += m_rotationSpeed * deltaTime;

		const float doublePi{ std::numbers::pi_v<float> *2.0f };

		if ( m_angle > doublePi )
		{
			m_angle -= doublePi;
		}
		else if ( m_angle < 0.f )
		{
			m_angle += doublePi;
		}

		// TODO L: RotatorComponent should support ellipses.

		auto& transform = GetOwner()->GetTransform();
		// x = cos(a) * r, y = sin(a) * r
		transform.SetLocalPosition( cos( m_angle ) * m_range, sin( m_angle ) * m_range );
	}
}
