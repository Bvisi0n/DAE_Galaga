#ifndef DAE_RENDERER_H
#define DAE_RENDERER_H

#include <cstdint>

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
		enum class PresentationMode : std::uint8_t
		{
			Letterbox,
			Overscan,
			IntegerScale
		};

		Renderer( const Renderer& ) = delete;
		Renderer( Renderer&& ) = delete;
		Renderer& operator=( const Renderer& ) = delete;
		Renderer& operator=( Renderer&& ) = delete;

		void Init( SDL_Window* window );
		void Render() const;
		void Destroy();

		[[nodiscard]] bool IsValid() const noexcept;
		[[nodiscard]] SDL_Renderer* GetSDLRenderer() const;

		void SetLogicalResolution( const glm::ivec2& resolution, PresentationMode mode = PresentationMode::Letterbox );
		[[nodiscard]] glm::ivec2 GetLogicalResolution() const noexcept;

		void RenderTexture( const Texture2D& texture, const glm::vec2& position ) const;
		void RenderTexture( const Texture2D& texture, const glm::vec2& position, const glm::vec2& size ) const;

		void SetBackgroundColor( const SDL_Color& color ) noexcept;
		[[nodiscard]] const SDL_Color& GetBackgroundColor() const;

	private:
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		glm::ivec2 m_logicalResolution{ 0, 0 };
		PresentationMode m_currentMode{ PresentationMode::Letterbox };
		SDL_Color m_clearColor{};

		friend class utils::Singleton<Renderer>;
		Renderer() = default;
		~Renderer() = default;
	};
}
#endif
