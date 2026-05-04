#ifndef DAE_PRIMITIVERENDERCOMPONENT_H
#define DAE_PRIMITIVERENDERCOMPONENT_H

#include <utility>
#include <variant>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

#include <Minigin/Core/Component.h>
#include <Minigin/Graphics/IRenderable.h>

struct SDL_Renderer;

namespace dae::core
{
	class GameObject;
}

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
		explicit PrimitiveRenderComponent( core::GameObject* owner, PrimitiveShape shape, SDL_FColor color, int thickness = 1 );

		~PrimitiveRenderComponent() override;

		PrimitiveRenderComponent( const PrimitiveRenderComponent& ) = delete;
		PrimitiveRenderComponent( PrimitiveRenderComponent&& ) = delete;
		PrimitiveRenderComponent& operator=( const PrimitiveRenderComponent& ) = delete;
		PrimitiveRenderComponent& operator=( PrimitiveRenderComponent&& ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( float /*deltaTime*/ ) override;
		void Render() const override;

		void SetEnabled( bool enabled ) noexcept;
		[[nodiscard]] bool IsEnabled() const noexcept;

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

		void RenderRect( SDL_Renderer* renderer, const RectShape& rect, float posX, float posY ) const;
		static void RenderCircle( SDL_Renderer* renderer, const CircleShape& circle, float posX, float posY );
		void RenderTriangle( SDL_Renderer* renderer, const TriangleShape& triangle, float posX, float posY ) const;
	};
}
#endif
