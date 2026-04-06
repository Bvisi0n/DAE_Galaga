#include <cassert>
#include <string>

#include <SDL3_ttf/SDL_ttf.h>

#include "Minigin/Graphics/Font.h"

namespace dae::graphics
{
	Font::Font( const std::string& fullPath, float size ) : m_pFont( nullptr )
	{
		m_pFont = TTF_OpenFont( fullPath.c_str(), size );
		assert( m_pFont != nullptr && "Failed to load font" );
	}

	Font::~Font()
	{
		if ( m_pFont )
		{
			TTF_CloseFont( m_pFont );
			m_pFont = nullptr;
		}
	}

	TTF_Font* Font::GetFont() const noexcept
	{
		return m_pFont;
	}
}
