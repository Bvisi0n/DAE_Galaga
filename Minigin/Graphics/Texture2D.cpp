#include <cassert>
#include <memory>
#include <string>

#include <glm/vec2.hpp>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Graphics/Texture2D.h"

namespace dae::graphics
{
	Texture2D::Texture2D( SDL_Texture* texture )
	{
		assert( texture != nullptr );
		m_texture.reset( texture );
	}

	Texture2D::Texture2D( const std::string& fullPath )
	{
		SDL_Surface* surface = SDL_LoadPNG( fullPath.c_str() );

		if ( surface == nullptr )
		{
			assert( surface != nullptr && "Failed to load PNG" );
			return;
		}

		SDL_Texture* raw = SDL_CreateTextureFromSurface( Renderer::GetInstance().GetSDLRenderer(), surface );

		SDL_DestroySurface( surface );

		if ( raw == nullptr )
		{
			assert( raw != nullptr && "Failed to create texture from surface" );
			return;
		}

		m_texture = std::unique_ptr<SDL_Texture, void( * )( SDL_Texture* )>( raw, SDL_DestroyTexture );
	}

	SDL_Texture* Texture2D::GetSDLTexture() const
	{
		return m_texture.get();
	}

	glm::vec2 Texture2D::GetSize() const
	{
		float width{};
		float height{};
		SDL_GetTextureSize( m_texture.get(), &width, &height );
		return glm::vec2{ width, height };
	}
}
