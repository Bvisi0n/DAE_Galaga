#ifndef PRIMITIVERENDERCOMPONENT_H
#define PRIMITIVERENDERCOMPONENT_H

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <algorithm>

#include <Minigin/Core/GameObject.h>
#include "Minigin/Core/Component.h"
#include "Minigin/Graphics/IRenderable.h"
#include "Minigin/Graphics/Renderer.h"

// Expand as needed, I kept the name generic, it should use some form of state to setup it's behaivour, don't want seperate components for each primitive type. (mentioned in State Pattern in the book)

namespace dae::graphics
{
	class PrimitiveRenderComponent final : public core::Component, public IRenderable
	{
	public:
		PrimitiveRenderComponent( core::GameObject* owner, const SDL_FRect& bounds, const SDL_Color& color, int thickness = 3 )
			: Component( owner )
			, m_bounds{ bounds }
			, m_color{ color }
			, m_thickness{ std::max( 1, thickness ) }
		{}

		virtual ~PrimitiveRenderComponent() = default;

		PrimitiveRenderComponent( const PrimitiveRenderComponent& ) = delete;
		PrimitiveRenderComponent( PrimitiveRenderComponent&& ) = delete;
		PrimitiveRenderComponent& operator=( const PrimitiveRenderComponent& ) = delete;
		PrimitiveRenderComponent& operator=( PrimitiveRenderComponent&& ) = delete;

		void InitializeLinkage() override
		{}

		void InitializeState() override
		{}

		void Update( const float ) override
		{}

		void Render() const override
		{
			if ( !m_enabled ) return;
			SDL_Renderer* renderer{ Renderer::GetInstance().GetSDLRenderer() };
			if ( !renderer ) return;
			SDL_SetRenderDrawColor( renderer, m_color.r, m_color.g, m_color.b, m_color.a );
			for ( int i = 0; i < m_thickness; ++i )
			{
				const float offset = static_cast<float>( i );
				const float x = m_bounds.x + offset;
				const float y = m_bounds.y + offset;
				const float w = m_bounds.w - offset * 2.0f;
				const float h = m_bounds.h - offset * 2.0f;
				if ( w <= 0.0f || h <= 0.0f ) break;
				SDL_FRect rect{ x, y, w, h };
				SDL_RenderRect( renderer, &rect );
			}
		}

		void SetBounds( const SDL_FRect& bounds )
		{
			m_bounds = bounds;
		}

		void SetEnabled( bool enabled )
		{
			m_enabled = enabled;
		}

		[[nodiscard]] bool IsEnabled() const
		{
			return m_enabled;
		}

	private:
		SDL_FRect m_bounds;
		SDL_Color m_color;
		int m_thickness;
		bool m_enabled{ true };
	};
}
#endif