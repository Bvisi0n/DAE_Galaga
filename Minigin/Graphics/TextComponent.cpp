#include <stdexcept>

#include <SDL3/SDL.h>

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"

#include "Minigin/Graphics/Font.h"
#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Graphics/Texture2D.h"

dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> pFont, const SDL_Color& color)
	: Component(pOwner)
	, m_text(text), m_pFont(std::move(pFont)), m_pTexture(nullptr), m_color(color), m_needsUpdate(true)
{
	assert(text.length() > 0 && "TextComponent requires non-empty text.");
}

void dae::TextComponent::Update(const float)
{
	if (m_needsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_text.c_str(), m_text.length(), m_color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_DestroySurface(surf);
		m_pTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_pTexture != nullptr && GetOwner() != nullptr)
	{
		const auto& position = GetOwner()->GetTransform().GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, position.x, position.y);
	}
}


void dae::TextComponent::SetColor(const SDL_Color& color)
{
	if (m_color.r != color.r || m_color.g != color.g || m_color.b != color.b || m_color.a != color.a)
	{
		m_color = color;
		m_needsUpdate = true;
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	if (m_text != text)
	{
		m_text = std::move(text);
		m_needsUpdate = true;
	}
}
