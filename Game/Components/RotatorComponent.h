#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H

#include "Minigin/Core/Component.h"

namespace dae::core
{
	class GameObject;
}

namespace bvi
{
	// Allows a GameObject to rotate around a parent in a circular path.
	class RotatorComponent final : public dae::core::Component
	{
	public:
		RotatorComponent( dae::core::GameObject* pOwner, float range, float speed );
		~RotatorComponent() override = default;

		void InitializeLinkage() override
		{}

		void InitializeState() override
		{}

		void Update( const float deltaTime ) override;

	private:
		float m_angle{ 0.0f };
		float m_range{ 0.0f };
		float m_rotationSpeed{ 0.0f };
	};
}
#endif
