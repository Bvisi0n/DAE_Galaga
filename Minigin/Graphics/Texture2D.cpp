#include <cassert>
#include <string>

#include <glm/ext/vector_float2.hpp>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Graphics/Texture2D.h"

namespace dae::graphics
{
	Texture2D::Texture2D( SDL_Texture* texture )
		: m_texture{ texture }
	{
		assert( m_texture != nullptr );
	}

	Texture2D::Texture2D( const std::string& fullPath )
	{
		SDL_Surface* surface = SDL_LoadPNG( fullPath.c_str() );

		if ( surface == nullptr )
		{
			m_texture = nullptr;

			assert( surface != nullptr && "Failed to load PNG" );
			return;
		}

		m_texture = SDL_CreateTextureFromSurface( Renderer::GetInstance().GetSDLRenderer(), surface );

		SDL_DestroySurface( surface );

		if ( m_texture == nullptr )
		{
			assert( m_texture != nullptr && "Failed to create texture from surface" );
			return;
		}
	}

	Texture2D::~Texture2D()
	{
		SDL_DestroyTexture( m_texture );
	}

	SDL_Texture* Texture2D::GetSDLTexture() const
	{
		return m_texture;
	}

	glm::vec2 Texture2D::GetSize() const
	{
		float width{};
		float height{};
		SDL_GetTextureSize( m_texture, &width, &height );
		return glm::vec2{ width, height };
	}
}
