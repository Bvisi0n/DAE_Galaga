#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <memory>
#include <string>

#include <glm/ext/vector_float2.hpp>

#include <SDL3/SDL_render.h>

struct SDL_Texture;

namespace dae::graphics
{
	class Texture2D final
	{
	public:
		explicit Texture2D( SDL_Texture* texture );
		explicit Texture2D( const std::string& fullPath );
		~Texture2D() = default;

		Texture2D( const Texture2D& ) = delete;
		Texture2D( Texture2D&& ) = delete;
		Texture2D& operator= ( const Texture2D& ) = delete;
		Texture2D& operator= ( const Texture2D&& ) = delete;

		[[nodiscard]] SDL_Texture* GetSDLTexture() const;
		[[nodiscard]] glm::vec2 GetSize() const;

	private:
		std::unique_ptr<SDL_Texture, void( * )( SDL_Texture* )> m_texture{ nullptr, SDL_DestroyTexture };
	};
}
#endif
