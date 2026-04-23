#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

#include <glm/vec3.hpp>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"

namespace dae::core
{
	class MoveComponent final : public Component
	{
	public:
		explicit MoveComponent( GameObject* owner, const float maxSpeed = 200.f, const float drag = 0.0f );
		~MoveComponent() override = default;

		MoveComponent( const MoveComponent& ) = delete;
		MoveComponent( MoveComponent&& ) = delete;
		MoveComponent& operator=( const MoveComponent& ) = delete;
		MoveComponent& operator=( MoveComponent&& ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float deltaTime ) override;
		void AddForce( const glm::vec3& force );

		void SetDragCoefficient( const float drag );
		[[nodiscard]] float GetDragCoefficient() const noexcept;

		void SetMaxSpeed( const float speed );

		void SetVelocity( const glm::vec3& velocity );
		[[nodiscard]] const glm::vec3& GetVelocity() const;

	private:
		glm::vec3 m_accumulatedForces{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_velocity{ 0.0f, 0.0f, 0.0f };
		float m_dragCoefficient{ 0.0f };
		float m_maxSpeedSq{ 20000.0f };
	};
}
#endif
