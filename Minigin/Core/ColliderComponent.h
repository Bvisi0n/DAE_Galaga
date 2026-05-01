#ifndef DAE_COLLIDERCOMPONENT_H
#define DAE_COLLIDERCOMPONENT_H

#include "Minigin/Core/Component.h"

namespace dae::core
{
	class GameObject;

	struct Rectangle
	{
		float x;
		float y;
		float width;
		float height;
	};

	struct Size2D
	{
		float width;
		float height;
	};

	class ColliderComponent final : public Component
	{
	public:
		explicit ColliderComponent( GameObject* owner, Size2D size, int layer );
		~ColliderComponent() override = default;

		ColliderComponent( const ColliderComponent& other ) = delete;
		ColliderComponent( ColliderComponent&& other ) = delete;
		ColliderComponent& operator=( const ColliderComponent& other ) = delete;
		ColliderComponent& operator=( ColliderComponent&& other ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;
		void Update( const float deltaTime ) override;

		[[nodiscard]] Rectangle GetWorldBounds() const;
		[[nodiscard]] int GetLayer() const noexcept;

	private:
		Rectangle m_localBounds;
		int m_layer;
	};
}

#endif
