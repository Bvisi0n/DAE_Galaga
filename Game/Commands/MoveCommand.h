#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <glm/glm.hpp>

#include "Minigin/Core/MoveComponent.h"
#include "Minigin/Input/Command.h"

namespace bvi::commands
{
	class MoveCommand final : public dae::input::Command
	{
	public:
		MoveCommand( dae::core::MoveComponent* moveComponent, const glm::vec3& direction )
			: m_moveComponent{ moveComponent }
			, m_direction{ direction }
		{}

		void Execute( const float ) override
		{
			if ( m_moveComponent )
			{
				m_moveComponent->AddDirection( m_direction );
			}
		}

		[[nodiscard]] const void* GetTargetContext() const override
		{
			return m_moveComponent;
		}

	private:
		dae::core::MoveComponent* m_moveComponent;
		glm::vec3 m_direction;
	};
}
#endif
