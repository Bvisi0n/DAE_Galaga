#ifndef DAE_COLLIDERCOMPONENT_H
#define DAE_COLLIDERCOMPONENT_H

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
		explicit ColliderComponent( GameObject* owner, float width, float height, int layer );
		~ColliderComponent() override = default;

		ColliderComponent( const ColliderComponent& other ) = delete;
		ColliderComponent( ColliderComponent&& other ) = delete;
		ColliderComponent& operator=( const ColliderComponent& other ) = delete;
		ColliderComponent& operator=( ColliderComponent&& other ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;
		void Update( const float deltaTime ) override;

		[[nodiscard]] Rect GetWorldBounds() const;
		[[nodiscard]] int GetLayer() const noexcept;

	private:
		Rect m_localBounds;
		int m_layer; // TODO dae_core - Optimize layers.
	};
}

#endif
