#include <stdexcept>

#include <SDL3_ttf/SDL_ttf.h>

#include "Font.h"

namespace dae
{
	Font::Font(const std::string& fullPath, float size) : m_pFont(nullptr)
	{
		m_pFont = TTF_OpenFont(fullPath.c_str(), size);
		if (m_pFont == nullptr) 
		{
			throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
		}
	}

	Font::~Font()
	{
		if (m_pFont)
		{
			TTF_CloseFont(m_pFont);
			m_pFont = nullptr;
		}
	}

	TTF_Font* Font::GetFont() const
	{
		return m_pFont;
	}
}
