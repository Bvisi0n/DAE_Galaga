#include <cmath>
#include <numbers>

#include <Game/Galaga/RotatorComponent.h>

#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/Transform.h>

namespace bvi::galaga
{
	RotatorComponent::RotatorComponent( dae::core::GameObject* owner, const RotatorDescriptor descriptor )
		: Component( owner )
		, m_angle( descriptor.initialAngle )
		, m_range( descriptor.range )
		, m_rotationSpeed( descriptor.speed )
	{}

	void RotatorComponent::InitializeLinkage()
	{}

	void RotatorComponent::InitializeState()
	{}

	void RotatorComponent::Update( const float deltaTime )
	{
		m_angle += m_rotationSpeed * deltaTime;

		const float doublePi{ std::numbers::pi_v<float> *2.0F };

		if ( m_angle > doublePi )
		{
			m_angle -= doublePi;
		}
		else if ( m_angle < 0.F )
		{
			m_angle += doublePi;
		}

		auto& transform = GetOwner()->GetTransform();
		transform.SetLocalPosition( cos( m_angle ) * m_range, sin( m_angle ) * m_range );
	}
}
