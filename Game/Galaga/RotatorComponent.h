#ifndef BVI_ROTATORCOMPONENT_H
#define BVI_ROTATORCOMPONENT_H

#include "Minigin/Core/Component.h"

namespace dae::core
{
	class GameObject;
}

namespace bvi::galaga
{
	class RotatorComponent final : public dae::core::Component
	{
	public:
		explicit RotatorComponent( dae::core::GameObject* owner, float range, float speed );
		~RotatorComponent() override = default;

		RotatorComponent( const RotatorComponent& other ) = delete;
		RotatorComponent( RotatorComponent&& other ) = delete;
		RotatorComponent& operator=( const RotatorComponent& other ) = delete;
		RotatorComponent& operator=( RotatorComponent&& other ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float deltaTime ) override;

	private:
		float m_angle{ 0.0f };
		float m_range{ 0.0f };
		float m_rotationSpeed{ 0.0f };
	};
}
#endif
