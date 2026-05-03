#include <memory>
#include <vector>

#include <Minigin/Core/ColliderComponent.h>
#include <Minigin/Core/CollisionSystem.h>
#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>

namespace dae::core
{
	void CollisionSystem::RegisterCallback( const CollisionCallback& callback )
	{
		m_callbacks.push_back( callback );
	}

	void CollisionSystem::Update( const std::vector<std::unique_ptr<GameObject>>& objects )
	{
		m_activeColliders.clear();

		for ( const auto& object : objects )
		{
			// TODO dae_core - This is an expensive operation, ColliderComponent should register themselves instead of being polled every frame.
			if ( auto* collider = object->GetComponent<ColliderComponent>() )
			{
				m_activeColliders.push_back( collider );
			}
		}

		const size_t count = m_activeColliders.size();

		for ( size_t indexA = 0; indexA < count; ++indexA )
		{
			auto* colliderA = m_activeColliders[ indexA ];
			const auto boundsA = colliderA->GetWorldBounds();

			for ( size_t indexB = indexA + 1; indexB < count; ++indexB )
			{
				auto* colliderB = m_activeColliders[ indexB ];

				if ( Intersects( boundsA, colliderB->GetWorldBounds() ) )
				{
					for ( auto& callback : m_callbacks )
					{
						callback( colliderA->GetOwner(), colliderB->GetOwner() );
					}
				}
			}
		}
	}

	bool CollisionSystem::Intersects( const Rectangle& rectangleA, const Rectangle& rectangleB ) noexcept
	{
		return
			(
				rectangleA.x < rectangleB.x + rectangleB.width
				&&
				rectangleA.x + rectangleA.width > rectangleB.x
				&&
				rectangleA.y < rectangleB.y + rectangleB.height
				&&
				rectangleA.y + rectangleA.height > rectangleB.y
			);
	}
}
