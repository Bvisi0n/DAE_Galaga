#ifndef BVI_SCREENWRAPCOMPONENT_H
#define BVI_SCREENWRAPCOMPONENT_H

#include <glm/ext/vector_float3.hpp>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"

// TODO bvi_gravity_bender - Fetch viewport dimensions instead of receiving them as parameters.

namespace bvi::gravity_bender
{
	class ScreenWrapComponent final : public dae::core::Component
	{
	public:
		explicit ScreenWrapComponent( dae::core::GameObject* owner, const float viewportWidth, const float viewportHeight, float spriteOffset = 0.0f )
			: dae::core::Component( owner )
			, m_viewportWidth( viewportWidth )
			, m_viewportHeight( viewportHeight )
			, m_spriteOffset( spriteOffset )
		{}

		~ScreenWrapComponent() override = default;

		ScreenWrapComponent( const ScreenWrapComponent& ) = delete;
		ScreenWrapComponent( ScreenWrapComponent&& ) = delete;
		ScreenWrapComponent& operator=( const ScreenWrapComponent& ) = delete;
		ScreenWrapComponent& operator=( ScreenWrapComponent&& ) = delete;

		void InitializeLinkage() override
		{}

		void InitializeState() override
		{}

		void Update( const float /*deltaTime*/ ) override
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

	private:
		float m_viewportWidth;
		float m_viewportHeight;
		float m_spriteOffset;
	};
}

#endif
