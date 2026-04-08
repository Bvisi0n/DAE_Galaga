#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <glm/glm.hpp>

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"
#include "Minigin/Input/Command.h"

namespace bvi
{
	class MoveCommand final : public dae::input::Command
	{
	public:
		MoveCommand( dae::core::GameObject* gameObject, const glm::vec2& direction, float speed )
			: m_gameObject( gameObject )
			, m_direction( direction )
			, m_speed( speed )
		{}

		void Execute( const float deltaTime ) override
		{
			// TODO L: Diagonal movement is currently double speed.
			auto& transform = m_gameObject->GetTransform();
			auto position = transform.GetLocalPosition();

			// TODO L: Use vector math.
			position.x += m_direction.x * m_speed * deltaTime;
			position.y += m_direction.y * m_speed * deltaTime;

			transform.SetLocalPosition( position );
		}

		[[nodiscard]] const void* GetTargetContext() const override
		{
			return m_gameObject;
		}

	private:
		dae::core::GameObject* m_gameObject;
		glm::vec2 m_direction;
		float m_speed;
	};
}
#endif
