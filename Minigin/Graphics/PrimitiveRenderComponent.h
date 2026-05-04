#ifndef DAE_PRIMITIVERENDERCOMPONENT_H
#define DAE_PRIMITIVERENDERCOMPONENT_H

#include <algorithm>
#include <array>
#include <type_traits>
#include <utility>
#include <variant>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Graphics/IRenderable.h>
#include <Minigin/Graphics/Renderer.h>

// TODO dae_input - Split into .cpp/.h

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
		explicit PrimitiveRenderComponent( core::GameObject* owner, PrimitiveShape shape, SDL_FColor color, int thickness = 1 )
			: Component( owner )
			, m_shape( shape )
			, m_color{ color }
			, m_thickness{ std::max( 1, thickness ) }
		{}

		~PrimitiveRenderComponent() override = default;

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
			if ( !m_enabled )
			{
				return;
			}
			SDL_Renderer* renderer{ Renderer::GetInstance().GetSDLRenderer() };
			if ( renderer == nullptr )
			{
				return;
			}

			SDL_SetRenderDrawColorFloat( renderer, m_color.r, m_color.g, m_color.b, m_color.a );

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
		SDL_FColor m_color;
		int m_thickness;
		bool m_enabled{ true };

		void RenderRect( SDL_Renderer* renderer, const RectShape& rect, float posX, float posY ) const
		{
			SDL_FRect drawRect{ posX + rect.bounds.x, posY + rect.bounds.y, rect.bounds.w, rect.bounds.h };
			if ( rect.isFilled )
			{
				SDL_RenderFillRect( renderer, &drawRect );
			}
			else
			{
				for ( int index = 0; index < m_thickness; ++index )
				{
					const auto offset = static_cast<float>( index );
					SDL_FRect thickRect{ drawRect.x + offset, drawRect.y + offset, drawRect.w - ( offset * 2.F ), drawRect.h - ( offset * 2.F ) };
					SDL_RenderRect( renderer, &thickRect );
				}
			}
		}

		static void RenderCircle( SDL_Renderer* renderer, const CircleShape& circle, float posX, float posY )
		{
			float circleX = circle.radius;
			float circleY = 0.F;
			float errorValue = 0.F;

			while ( circleX >= circleY )
			{
				if ( circle.isFilled )
				{
					SDL_RenderLine( renderer, posX - circleX, posY + circleY, posX + circleX, posY + circleY );
					SDL_RenderLine( renderer, posX - circleY, posY + circleX, posX + circleY, posY + circleX );
					SDL_RenderLine( renderer, posX - circleX, posY - circleY, posX + circleX, posY - circleY );
					SDL_RenderLine( renderer, posX - circleY, posY - circleX, posX + circleY, posY - circleX );
				}
				else
				{
					SDL_RenderPoint( renderer, posX + circleX, posY + circleY );
					SDL_RenderPoint( renderer, posX + circleY, posY + circleX );
					SDL_RenderPoint( renderer, posX - circleY, posY + circleX );
					SDL_RenderPoint( renderer, posX - circleX, posY + circleY );
					SDL_RenderPoint( renderer, posX - circleX, posY - circleY );
					SDL_RenderPoint( renderer, posX - circleY, posY - circleX );
					SDL_RenderPoint( renderer, posX + circleY, posY - circleX );
					SDL_RenderPoint( renderer, posX + circleX, posY - circleY );
				}

				if ( errorValue <= 0.F )
				{
					circleY += 1.F;
					errorValue += ( 2.F * circleY ) + 1.F;
				}

				if ( errorValue > 0.F )
				{
					circleX -= 1.F;
					errorValue -= ( 2.F * circleX ) + 1.F;
				}
			}
		}

		void RenderTriangle( SDL_Renderer* renderer, const TriangleShape& triangle, float posX, float posY ) const
		{
			if ( triangle.isFilled )
			{
				const std::array<SDL_Vertex, 3> vertices
				{ {
					{
						.position = { posX + triangle.p1.x, posY + triangle.p1.y },
						.color = m_color,
						.tex_coord = { 0.F, 0.F }
					},
					{
						.position = { posX + triangle.p2.x, posY + triangle.p2.y },
						.color = m_color,
						.tex_coord = { 0.F, 0.F }
					},
					{
						.position = { posX + triangle.p3.x, posY + triangle.p3.y },
						.color = m_color,
						.tex_coord = { 0.F, 0.F }
					}
				} };
				SDL_RenderGeometry( renderer, nullptr, vertices.data(), 3, nullptr, 0 );
			}
			else
			{
				SDL_RenderLine( renderer, posX + triangle.p1.x, posY + triangle.p1.y, posX + triangle.p2.x, posY + triangle.p2.y );
				SDL_RenderLine( renderer, posX + triangle.p2.x, posY + triangle.p2.y, posX + triangle.p3.x, posY + triangle.p3.y );
				SDL_RenderLine( renderer, posX + triangle.p3.x, posY + triangle.p3.y, posX + triangle.p1.x, posY + triangle.p1.y );
			}
		}
	};
}
#endif
