#ifndef PRIMITIVERENDERCOMPONENT_H
#define PRIMITIVERENDERCOMPONENT_H

#include <algorithm>
#include <type_traits>
#include <utility>
#include <variant>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include <Minigin/Core/GameObject.h>
#include "Minigin/Core/Component.h"
#include "Minigin/Graphics/IRenderable.h"
#include "Minigin/Graphics/Renderer.h"

namespace dae::graphics
{
	struct RectShape
	{
		SDL_FRect bounds;
		bool isFilled;
	};

	struct CircleShape
	{
		float radius;
		bool isFilled;
	};

	struct TriangleShape
	{
		SDL_FPoint p1, p2, p3;
		bool isFilled;
	};

	using PrimitiveShape = std::variant<RectShape, CircleShape, TriangleShape>;

	class PrimitiveRenderComponent final : public core::Component, public IRenderable
	{
	public:
		PrimitiveRenderComponent( core::GameObject* owner, PrimitiveShape shape, SDL_Color color, int thickness = 1 )
			: Component( owner )
			, m_shape( std::move( shape ) )
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

		void Update( const float /*deltaTime*/ ) override
		{}

		void Render() const override
		{
			if ( !m_enabled ) return;
			SDL_Renderer* renderer{ Renderer::GetInstance().GetSDLRenderer() };
			if ( !renderer ) return;

			SDL_SetRenderDrawColor( renderer, m_color.r, m_color.g, m_color.b, m_color.a );

			const auto worldPos = GetOwner()->GetTransform().GetWorldPosition();
			const float posX = worldPos.x;
			const float posY = worldPos.y;

			std::visit( [ this, renderer, posX, posY ] ( const auto& shape )
				{
					using ShapeType = std::decay_t<decltype( shape )>;

					if constexpr ( std::is_same_v<ShapeType, RectShape> )
					{
						RenderRect( renderer, shape, posX, posY );
					}
					else if constexpr ( std::is_same_v<ShapeType, CircleShape> )
					{
						RenderCircle( renderer, shape, posX, posY );
					}
					else if constexpr ( std::is_same_v<ShapeType, TriangleShape> )
					{
						RenderTriangle( renderer, shape, posX, posY );
					}
				}, m_shape );
		}

		void SetEnabled( bool enabled ) noexcept
		{
			m_enabled = enabled;
		}

		[[nodiscard]] bool IsEnabled() const noexcept
		{
			return m_enabled;
		}

		template<typename T>
		void SetShape( T&& shape ) noexcept
		{
			m_shape = std::forward<T>( shape );
		}

		template<typename T>
		[[nodiscard]] T* GetShapeAs() noexcept
		{
			return std::get_if<T>( &m_shape );
		}

	private:
		PrimitiveShape m_shape;
		SDL_Color m_color;
		int m_thickness;
		bool m_enabled{ true };

		void RenderRect( SDL_Renderer* renderer, const RectShape& rect, float x, float y ) const
		{
			SDL_FRect drawRect{ x + rect.bounds.x, y + rect.bounds.y, rect.bounds.w, rect.bounds.h };
			if ( rect.isFilled )
			{
				SDL_RenderFillRect( renderer, &drawRect );
			}
			else
			{
				for ( int i = 0; i < m_thickness; ++i )
				{
					const float offset = static_cast<float>( i );
					SDL_FRect thickRect{ drawRect.x + offset, drawRect.y + offset, drawRect.w - offset * 2.f, drawRect.h - offset * 2.f };
					SDL_RenderRect( renderer, &thickRect );
				}
			}
		}

		void RenderCircle( SDL_Renderer* renderer, const CircleShape& circle, float x, float y ) const
		{
			float cx = circle.radius;
			float cy = 0;
			float err = 0;

			while ( cx >= cy )
			{
				if ( circle.isFilled )
				{
					SDL_RenderLine( renderer, x - cx, y + cy, x + cx, y + cy );
					SDL_RenderLine( renderer, x - cy, y + cx, x + cy, y + cx );
					SDL_RenderLine( renderer, x - cx, y - cy, x + cx, y - cy );
					SDL_RenderLine( renderer, x - cy, y - cx, x + cy, y - cx );
				}
				else
				{
					SDL_RenderPoint( renderer, x + cx, y + cy );
					SDL_RenderPoint( renderer, x + cy, y + cx );
					SDL_RenderPoint( renderer, x - cy, y + cx );
					SDL_RenderPoint( renderer, x - cx, y + cy );
					SDL_RenderPoint( renderer, x - cx, y - cy );
					SDL_RenderPoint( renderer, x - cy, y - cx );
					SDL_RenderPoint( renderer, x + cy, y - cx );
					SDL_RenderPoint( renderer, x + cx, y - cy );
				}

				if ( err <= 0 )
				{
					cy += 1;
					err += 2 * cy + 1;
				}

				if ( err > 0 )
				{
					cx -= 1;
					err -= 2 * cx + 1;
				}
			}
		}

		void RenderTriangle( SDL_Renderer* renderer, const TriangleShape& tri, float x, float y ) const
		{
			if ( tri.isFilled )
			{
				SDL_Vertex vertices[ 3 ] = {
					{ {x + tri.p1.x, y + tri.p1.y}, {m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, m_color.a / 255.f}, {0,0} },
					{ {x + tri.p2.x, y + tri.p2.y}, {m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, m_color.a / 255.f}, {0,0} },
					{ {x + tri.p3.x, y + tri.p3.y}, {m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, m_color.a / 255.f}, {0,0} }
				};
				SDL_RenderGeometry( renderer, nullptr, vertices, 3, nullptr, 0 );
			}
			else
			{
				SDL_RenderLine( renderer, x + tri.p1.x, y + tri.p1.y, x + tri.p2.x, y + tri.p2.y );
				SDL_RenderLine( renderer, x + tri.p2.x, y + tri.p2.y, x + tri.p3.x, y + tri.p3.y );
				SDL_RenderLine( renderer, x + tri.p3.x, y + tri.p3.y, x + tri.p1.x, y + tri.p1.y );
			}
		}
	};
}
#endif
