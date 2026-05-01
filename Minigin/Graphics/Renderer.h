#ifndef DAE_RENDERER_H
#define DAE_RENDERER_H

#include <glm/vec2.hpp>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <Minigin/Utilities/Singleton.h>

namespace dae::graphics
{
	class Texture2D;

	class Renderer final : public utils::Singleton<Renderer>
	{
	public:
		void Init( SDL_Window* window );
		void Render() const;
		void Destroy();

		[[nodiscard]] bool IsValid() const noexcept;

		void RenderTexture( const Texture2D& texture, const glm::vec2& position ) const;
		void RenderTexture( const Texture2D& texture, const glm::vec2& position, const glm::vec2& size ) const;

		void SetBackgroundColor( const SDL_Color& color ) noexcept;

		[[nodiscard]] SDL_Renderer* GetSDLRenderer() const;
		[[nodiscard]] const SDL_Color& GetBackgroundColor() const;

	private:
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};

		friend class utils::Singleton<Renderer>;
		Renderer() = default;
		virtual ~Renderer() = default;
	};
}
#endif
