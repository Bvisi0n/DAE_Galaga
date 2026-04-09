#ifndef PRIMITIVERENDERCOMPONENT_H
#define PRIMITIVERENDERCOMPONENT_H

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

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
		PrimitiveRenderComponent( core::GameObject* owner, const SDL_FRect& bounds, const SDL_Color& color )
			: Component( owner )
			, m_bounds{ bounds }
			, m_color{ color }
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
			SDL_Renderer* renderer{ Renderer::GetInstance().GetSDLRenderer() };
			if ( !renderer ) return;
			SDL_SetRenderDrawColor( renderer, m_color.r, m_color.g, m_color.b, m_color.a );
			SDL_RenderRect( renderer, &m_bounds );
		}

	private:
		SDL_FRect m_bounds;
		SDL_Color m_color;
	};
}
#endif