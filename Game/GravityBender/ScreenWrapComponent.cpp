#include <glm/ext/vector_float3.hpp>

#include "Game/GravityBender/ScreenWrapComponent.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"

namespace bvi::gravity_bender
{
	ScreenWrapComponent::ScreenWrapComponent( dae::core::GameObject* owner, const float viewportWidth, const float viewportHeight, float spriteOffset )
		: dae::core::Component( owner )
		, m_viewportWidth( viewportWidth )
		, m_viewportHeight( viewportHeight )
		, m_spriteOffset( spriteOffset )
	{}

	ScreenWrapComponent::~ScreenWrapComponent() = default;

	void ScreenWrapComponent::InitializeLinkage()
	{}

	void ScreenWrapComponent::InitializeState()
	{}

	void ScreenWrapComponent::Update( const float /*deltaTime*/ )
	{
		auto& transform = GetOwner()->GetTransform();

		glm::vec3 currentPos = transform.GetLocalPosition();
		bool hasWrapped = false;

		if ( currentPos.x < -m_spriteOffset )
		{
			currentPos.x += ( m_viewportWidth + ( m_spriteOffset * 2.0f ) );
			hasWrapped = true;
		}
		else if ( currentPos.x > m_viewportWidth + m_spriteOffset )
		{
			currentPos.x -= ( m_viewportWidth + ( m_spriteOffset * 2.0f ) );
			hasWrapped = true;
		}

		if ( currentPos.y < -m_spriteOffset )
		{
			currentPos.y += ( m_viewportHeight + ( m_spriteOffset * 2.0f ) );
			hasWrapped = true;
		}
		else if ( currentPos.y > m_viewportHeight + m_spriteOffset )
		{
			currentPos.y -= ( m_viewportHeight + ( m_spriteOffset * 2.0f ) );
			hasWrapped = true;
		}

		if ( hasWrapped )
		{
			transform.SetLocalPosition( currentPos );
		}
	}
}
