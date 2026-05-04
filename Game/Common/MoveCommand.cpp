#include <glm/ext/vector_float3.hpp>

#include <Minigin/Core/MoveComponent.h>

#include <Game/Common/MoveCommand.h>

namespace bvi::common
{
	MoveCommand::MoveCommand( dae::core::MoveComponent* moveComponent, const glm::vec3& thrustVector )
		: m_moveComponent{ moveComponent }
		, m_thrustVector{ thrustVector }
	{}

	void MoveCommand::Execute( const float /*deltaTime*/ )
	{
		if ( m_moveComponent != nullptr )
		{
			m_moveComponent->AddForce( m_thrustVector );
		}
	}

	const void* MoveCommand::GetTargetContext() const
	{
		return m_moveComponent;
	}
}
