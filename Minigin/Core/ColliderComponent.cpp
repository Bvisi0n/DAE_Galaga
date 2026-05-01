#include "Minigin/Core/ColliderComponent.h"
#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"

namespace dae::core
{
	ColliderComponent::ColliderComponent( GameObject* owner, Size2D size, int layer )
		: Component( owner )
		, m_localBounds{ .x = 0.F, .y = 0.F, .width = size.width, .height = size.height }
		, m_layer( layer )
	{}

	void ColliderComponent::InitializeLinkage()
	{}

	void ColliderComponent::InitializeState()
	{}

	void ColliderComponent::Update( const float /*deltaTime*/ )
	{}

	Rectangle ColliderComponent::GetWorldBounds() const
	{
		const auto& position = GetOwner()->GetTransform().GetWorldPosition();

		return Rectangle
		{
			.x = position.x,
			.y = position.y,
			.width = m_localBounds.width,
			.height = m_localBounds.height
		};
	}

	int ColliderComponent::GetLayer() const noexcept
	{
		return m_layer;
	}
}
