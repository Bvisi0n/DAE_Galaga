#include <cassert>
#include <memory>
#include <string>

#include <SDL3_ttf/SDL_ttf.h>

#include "Minigin/Graphics/Font.h"

namespace dae::graphics
{
	Font::Font( const std::string& fullPath, float size )
	{
		TTF_Font* raw = TTF_OpenFont( fullPath.c_str(), size );
		assert( raw != nullptr && "Failed to load font" );
		m_font = std::unique_ptr<TTF_Font, void( * )( TTF_Font* )>( raw, TTF_CloseFont );
	}

	TTF_Font* Font::GetFont() const noexcept
	{
		return m_font.get();
	}
}
