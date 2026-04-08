#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "Minigin/Core/Component.h"

namespace dae::core
{
	class GameObject;

	struct Rect
	{
		// ES.10: Declare one name (only) per declaration

		float x;
		float y;
		float width;
		float height;
	};

	class ColliderComponent final : public Component
	{
	public:
		ColliderComponent( GameObject* owner, float width, float height, int layer );

		[[nodiscard]] Rect GetWorldBounds() const;
		[[nodiscard]] int GetLayer() const noexcept;

	protected:
		void InitializeLinkage() override
		{}

		void InitializeState() override
		{}

		void Update( const float ) override
		{}

	private:
		Rect m_localBounds;
		int m_Layer; // TODO L: Optimize layers.
	};
}

#endif
