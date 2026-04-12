#include <memory>
#include <vector>

#include "Minigin/Core/ColliderComponent.h"
#include "Minigin/Core/CollisionSystem.h"
#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"

namespace dae::core
{
	void CollisionSystem::RegisterCallback( CollisionCallback callback )
	{
		m_callbacks.push_back( callback );
	}

	void CollisionSystem::Update( const std::vector<std::unique_ptr<GameObject>>& objects )
	{
		std::vector<ColliderComponent*> activeColliders;
		for ( const auto& obj : objects )
		{
			if ( auto col = obj->GetComponent<ColliderComponent>() )
			{
				activeColliders.push_back( col );
			}
		}

		// TODO L: Are these old for loops because iterator invalidation?
			// I think they can be range-based... Testing required.
		for ( size_t i = 0; i < activeColliders.size(); ++i )
		{
			for ( size_t j = i + 1; j < activeColliders.size(); ++j )
			{
				auto* colA = activeColliders[ i ];
				auto* colB = activeColliders[ j ];

				if ( Intersects( colA->GetWorldBounds(), colB->GetWorldBounds() ) )
				{
					for ( auto& cb : m_callbacks )
					{
						cb( colA->GetOwner(), colB->GetOwner() );
					}
				}
			}
		}
	}

	bool CollisionSystem::Intersects( const Rect& a, const Rect& b ) const noexcept
	{
		return ( a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y );
	}
}
