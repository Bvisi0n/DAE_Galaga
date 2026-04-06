#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "Minigin/Utilities/Singleton.h"

namespace dae::graphics
{
	class Texture2D;

	// Simple RAII wrapper for the SDL renderer
	class Renderer final : public utils::Singleton<Renderer>
	{
	public:
		void Init( SDL_Window* pWindow );
		void Render() const;
		void Destroy();

		[[nodiscard]] bool IsValid() const noexcept;

		void RenderTexture( const Texture2D& texture, float x, float y ) const;
		void RenderTexture( const Texture2D& texture, float x, float y, float width, float height ) const;

		void SetBackgroundColor( const SDL_Color& color ) noexcept;


		[[nodiscard]] SDL_Renderer* GetSDLRenderer() const;
		[[nodiscard]] const SDL_Color& GetBackgroundColor() const;


	private:
		SDL_Renderer* m_pRenderer{};
		SDL_Window* m_pWindow{};
		SDL_Color m_clearColor{};

		friend class utils::Singleton<Renderer>;
		Renderer() = default;
		virtual ~Renderer() = default;
	};
}
#endif
