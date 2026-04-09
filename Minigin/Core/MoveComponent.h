#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

#include <glm/ext/vector_float3.hpp>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"

namespace dae::core
{
	class MoveComponent final : public Component
	{
	public:
		MoveComponent( GameObject* owner, const float maxSpeed, const bool resetDirectionPerFrame = true );
		virtual ~MoveComponent() = default;

		MoveComponent( const MoveComponent& ) = delete;
		MoveComponent( MoveComponent&& ) = delete;
		MoveComponent& operator=( const MoveComponent& ) = delete;
		MoveComponent& operator=( MoveComponent&& ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float deltaTime ) override;

		void AddDirection( const glm::vec3& direction );

		void SetSpeed( const float speed );
		[[nodiscard]] float GetSpeed() const;

	private:
		float m_maxSpeed{ 100.0f };
		glm::vec3 m_accumulatedDirection{ 0.0f, 0.0f, 0.0f };
		bool m_resetDirectionPerFrame;
	};
}
#endif
