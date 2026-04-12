#include "Minigin/Core/ColliderComponent.h"
#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"

namespace dae::core
{
	ColliderComponent::ColliderComponent( GameObject* owner, float width, float height, int layer )
		: Component( owner )
		, m_localBounds{ 0, 0, width, height }
		, m_layer( layer )
	{}

	void ColliderComponent::InitializeLinkage()
	{}

	void ColliderComponent::InitializeState()
	{}

	void ColliderComponent::Update( const float /*deltaTime*/ )
	{}

	Rect ColliderComponent::GetWorldBounds() const
	{
		const auto& pos = GetOwner()->GetTransform().GetWorldPosition();
		return Rect{ pos.x, pos.y, m_localBounds.width, m_localBounds.height };
	}

	int ColliderComponent::GetLayer() const noexcept
	{
		return m_layer;
	}
}
