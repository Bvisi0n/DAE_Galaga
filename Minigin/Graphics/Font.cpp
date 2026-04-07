#include <cassert>
#include <string>

#include <SDL3_ttf/SDL_ttf.h>

#include "Minigin/Graphics/Font.h"

namespace dae::graphics
{
	Font::Font( const std::string& fullPath, float size ) : m_font( nullptr )
	{
		m_font = TTF_OpenFont( fullPath.c_str(), size );
		assert( m_font != nullptr && "Failed to load font" );
	}

	Font::~Font()
	{
		if ( m_font )
		{
			TTF_CloseFont( m_font );
			m_font = nullptr;
		}
	}

	TTF_Font* Font::GetFont() const noexcept
	{
		return m_font;
	}
}
