#ifndef DAE_COLLISIONSYSTEM_H
#define DAE_COLLISIONSYSTEM_H

#include <functional>
#include <memory>
#include <vector>

namespace dae::core
{
	class ColliderComponent;
	class GameObject;
	struct Rectangle;

	class CollisionSystem final
	{
	public:
		using CollisionCallback = std::function<void( GameObject*, GameObject* )>;

		void RegisterCallback( const CollisionCallback& callback );

		void Update( const std::vector<std::unique_ptr<GameObject>>& objects );

	private:
		std::vector<CollisionCallback> m_callbacks{};
		std::vector<ColliderComponent*> m_activeColliders{};

		[[nodiscard]] static bool Intersects( const Rectangle& rectangleA, const Rectangle& rectangleB ) noexcept;
	};
}

#endif
