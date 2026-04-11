#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <glm/vec3.hpp>

#include "Minigin/Core/MoveComponent.h"
#include "Minigin/Input/Command.h"

namespace bvi::commands
{
	class MoveCommand final : public dae::input::Command
	{
	public:
		MoveCommand( dae::core::MoveComponent* moveComponent, const glm::vec3& thrustVector )
			: m_moveComponent{ moveComponent }
			, m_thrustVector{ thrustVector }
		{}

		void Execute( const float ) override
		{
			if ( m_moveComponent )
			{
				m_moveComponent->AddForce( m_thrustVector );
			}
		}

		[[nodiscard]] const void* GetTargetContext() const override
		{
			return m_moveComponent;
		}

	private:
		dae::core::MoveComponent* m_moveComponent{ nullptr };
		glm::vec3 m_thrustVector{ 0.0f, 0.0f, 0.0f };
	};
}
#endif
