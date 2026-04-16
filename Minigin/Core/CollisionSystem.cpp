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
		m_activeColliders.clear();

		for ( const auto& obj : objects )
		{
			// TODO L: This is an expensive operation, ColliderComponent should register themselves instead of being polled every frame.
			if ( auto* col = obj->GetComponent<ColliderComponent>() )
			{
				m_activeColliders.push_back( col );
			}
		}

		const size_t count = m_activeColliders.size();

		for ( size_t i = 0; i < count; ++i )
		{
			auto* colA = m_activeColliders[ i ];
			const auto boundsA = colA->GetWorldBounds();

			for ( size_t j = i + 1; j < count; ++j )
			{
				auto* colB = m_activeColliders[ j ];

				if ( Intersects( boundsA, colB->GetWorldBounds() ) )
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
