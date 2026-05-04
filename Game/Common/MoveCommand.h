#ifndef BVI_MOVECOMMAND_H
#define BVI_MOVECOMMAND_H

#include <glm/ext/vector_float3.hpp>

#include <Minigin/Input/ICommand.h>

namespace dae::core
{
	class MoveComponent;
}

namespace bvi::common
{
	class MoveCommand final : public dae::input::ICommand
	{
	public:
		MoveCommand( dae::core::MoveComponent* moveComponent, const glm::vec3& thrustVector );

		void Execute( float /*deltaTime*/ ) override;
		[[nodiscard]] const void* GetTargetContext() const override;

	private:
		dae::core::MoveComponent* m_moveComponent{ nullptr };
		glm::vec3 m_thrustVector{ 0.F, 0.F, 0.F };
	};
}
#endif
