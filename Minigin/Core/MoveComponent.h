#ifndef DAE_MOVECOMPONENT_H
#define DAE_MOVECOMPONENT_H

#include <glm/vec3.hpp>

#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>

namespace dae::core
{
	struct MoveDescriptor
	{
		float maxSpeed{ 200.F };
		float drag{ 0.F };
	};

	class MoveComponent final : public Component
	{
	public:
		explicit MoveComponent( GameObject* owner, const MoveDescriptor& descriptor = {} );
		~MoveComponent() override = default;

		MoveComponent( const MoveComponent& ) = delete;
		MoveComponent( MoveComponent&& ) = delete;
		MoveComponent& operator=( const MoveComponent& ) = delete;
		MoveComponent& operator=( MoveComponent&& ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( float deltaTime ) override;
		void AddForce( const glm::vec3& force );

		void SetDragCoefficient( float drag );
		[[nodiscard]] float GetDragCoefficient() const noexcept;

		void SetMaxSpeed( float speed );

		void SetVelocity( const glm::vec3& velocity );
		[[nodiscard]] const glm::vec3& GetVelocity() const;

	private:
		glm::vec3 m_accumulatedForces{ 0.F, 0.F, 0.F };
		glm::vec3 m_velocity{ 0.F, 0.F, 0.F };
		float m_dragCoefficient{ 0.F };
		float m_maxSpeedSq{ 20000.F };
	};
}
#endif
