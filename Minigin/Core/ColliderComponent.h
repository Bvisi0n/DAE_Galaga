#ifndef DAE_COLLIDERCOMPONENT_H
#define DAE_COLLIDERCOMPONENT_H

#include <cstdint>

#include <Minigin/Core/Component.h>

// TODO dae_core - Use SDMHash for CollisionTag instead to pull game specific code out of engine.

namespace dae::core
{
	class GameObject;

	enum class CollisionTag : std::uint8_t
	{
		Default = 0,
		Player = 1,
		Unit = 2,
		GravityWell = 3
	};

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
		void Update( float deltaTime ) override;

		[[nodiscard]] Rectangle GetWorldBounds() const;
		[[nodiscard]] int GetLayer() const noexcept;

		void SetCollisionTag( CollisionTag tag ) noexcept;
		[[nodiscard]] CollisionTag GetCollisionTag() const noexcept;

	private:
		Rectangle m_localBounds;
		int m_layer;
		CollisionTag m_tag{ CollisionTag::Default };
	};
}

#endif
