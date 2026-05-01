#ifndef BVI_ROTATORCOMPONENT_H
#define BVI_ROTATORCOMPONENT_H

#include "Minigin/Core/Component.h"

namespace dae::core
{
	class GameObject;
}

namespace bvi::galaga
{
	struct RotatorDescriptor
	{
		float range{ 0.0f };
		float speed{ 0.0f };
		float initialAngle{ 0.0f };
	};

	class RotatorComponent final : public dae::core::Component
	{
	public:
		explicit RotatorComponent( dae::core::GameObject* owner, const RotatorDescriptor descriptor );
		~RotatorComponent() override = default;

		RotatorComponent( const RotatorComponent& other ) = delete;
		RotatorComponent( RotatorComponent&& other ) = delete;
		RotatorComponent& operator=( const RotatorComponent& other ) = delete;
		RotatorComponent& operator=( RotatorComponent&& other ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float deltaTime ) override;

	private:
		float m_angle;
		float m_range;
		float m_rotationSpeed;
	};
}
#endif
