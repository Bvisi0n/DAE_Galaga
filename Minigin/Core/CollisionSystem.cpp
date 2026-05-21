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

			if ( colliderA->GetOwner()->IsPendingDeletion() )
			{
				continue;
			}

			const auto boundsA = colliderA->GetWorldBounds();

			for ( size_t indexB = indexA + 1; indexB < count; ++indexB )
			{
				ProcessCollisionPair( colliderA, boundsA, m_activeColliders[ indexB ] );
			}
		}
	}

	void CollisionSystem::ProcessCollisionPair( ColliderComponent* colliderA, const Rectangle& boundsA, ColliderComponent* colliderB )
	{
		if ( colliderB->GetOwner()->IsPendingDeletion() )
		{
			return;
		}

		const auto tagA = colliderA->GetCollisionTag();
		const auto tagB = colliderB->GetCollisionTag();

		if ( tagA == CollisionTag::Unit && tagB == CollisionTag::Unit )
		{
			return;
		}

		// TODO dae_core - Game code in the engine! Temporary solution.

		if ( Intersects( boundsA, colliderB->GetWorldBounds() ) )
		{
			const bool unitBMustDie = ( tagA == CollisionTag::GravityWell && tagB == CollisionTag::Unit ) || ( tagA == CollisionTag::Unit && tagB == CollisionTag::Player );

			const bool unitAMustDie = ( tagB == CollisionTag::GravityWell && tagA == CollisionTag::Unit ) || ( tagB == CollisionTag::Unit && tagA == CollisionTag::Player );

			if ( unitBMustDie )
			{
				colliderB->GetOwner()->MarkForDeletion();
			}
			else if ( unitAMustDie )
			{
				colliderA->GetOwner()->MarkForDeletion();
			}

			for ( auto& callback : m_callbacks )
			{
				callback( colliderA->GetOwner(), colliderB->GetOwner() );
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
