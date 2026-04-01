#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>

#include <SDL3/SDL.h>

#include "Singletons/Singleton.h"

namespace dae
{
	class Texture2D;

	// Simple RAII wrapper for the SDL renderer
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* pWindow);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		void SetBackgroundColor(const SDL_Color& color) noexcept
		{
			m_clearColor = color;
		}

		[[nodiscard]] SDL_Renderer* GetSDLRenderer() const;
		[[nodiscard]] const SDL_Color& GetBackgroundColor() const
		{
			return m_clearColor;
		}

	private:
		SDL_Renderer* m_pRenderer{};
		SDL_Window* m_pWindow{};
		SDL_Color m_clearColor{};

		friend class Singleton<Renderer>;
		Renderer() = default;
		virtual ~Renderer() = default;
	};
}
#endif
