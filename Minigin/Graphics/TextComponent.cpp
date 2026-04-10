#include <cassert>
#include <memory>
#include <string>
#include <utility>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"

#include "Minigin/Graphics/Font.h"
#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Graphics/Texture2D.h"

namespace dae::graphics
{
	TextComponent::TextComponent( core::GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color )
		: Component( owner )
		, m_text( text ), m_font( std::move( font ) ), m_texture( nullptr ), m_color( color ), m_needsUpdate( true )
	{
		assert( text.length() > 0 && "TextComponent requires non-empty text." );
	}

	void TextComponent::InitializeLinkage()
	{}

	void TextComponent::InitializeState()
	{}

	void TextComponent::Update( const float )
	{
		if ( !m_needsUpdate )
		{
			return;
		}

		if ( m_font == nullptr || m_font->GetFont() == nullptr )
		{
			return;
		}

		SDL_Surface* surface = TTF_RenderText_Blended( m_font->GetFont(), m_text.c_str(), static_cast<int>( m_text.length() ), m_color );

		if ( surface == nullptr )
		{
			assert( surface != nullptr && "Render text failed" );
			return;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface( Renderer::GetInstance().GetSDLRenderer(), surface );

		SDL_DestroySurface( surface );

		if ( texture == nullptr )
		{
			assert( texture != nullptr && "Create text texture from surface failed" );
			return;
		}

		m_texture = std::make_shared<Texture2D>( texture );
		m_needsUpdate = false;
	}

	void TextComponent::Render() const
	{
		if ( m_texture != nullptr && GetOwner() != nullptr )
		{
			const auto& position = GetOwner()->GetTransform().GetWorldPosition();
			Renderer::GetInstance().RenderTexture( *m_texture, position.x, position.y );
		}
	}


	void TextComponent::SetColor( const SDL_Color& color )
	{
		if ( m_color.r != color.r || m_color.g != color.g || m_color.b != color.b || m_color.a != color.a )
		{
			m_color = color;
			m_needsUpdate = true;
		}
	}

	void TextComponent::SetText( const std::string& text )
	{
		if ( m_text != text )
		{
			m_text = text;
			m_needsUpdate = true;
		}
	}
}
