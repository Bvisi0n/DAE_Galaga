#include <algorithm>
#include <array>
#include <type_traits>
#include <variant>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/Transform.h>
#include <Minigin/Graphics/PrimitiveRenderComponent.h>
#include <Minigin/Graphics/Renderer.h>

namespace dae::graphics
{
	PrimitiveRenderComponent::PrimitiveRenderComponent( core::GameObject* owner, PrimitiveShape shape, SDL_FColor color, int thickness )
		: Component( owner )
		, m_shape( shape )
		, m_color{ color }
		, m_thickness{ std::max( 1, thickness ) }
	{}

	PrimitiveRenderComponent::~PrimitiveRenderComponent() = default;

	void PrimitiveRenderComponent::InitializeLinkage()
	{}

	void PrimitiveRenderComponent::InitializeState()
	{}

	void PrimitiveRenderComponent::Update( const float /*deltaTime*/ )
	{}

	void PrimitiveRenderComponent::Render() const
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

	void PrimitiveRenderComponent::SetEnabled( bool enabled ) noexcept
	{
		m_enabled = enabled;
	}

	bool PrimitiveRenderComponent::IsEnabled() const noexcept
	{
		return m_enabled;
	}

	void PrimitiveRenderComponent::RenderRect( SDL_Renderer* renderer, const RectShape& rect, float posX, float posY ) const
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

	void PrimitiveRenderComponent::RenderCircle( SDL_Renderer* renderer, const CircleShape& circle, float posX, float posY )
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

	void PrimitiveRenderComponent::RenderTriangle( SDL_Renderer* renderer, const TriangleShape& triangle, float posX, float posY ) const
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
}
