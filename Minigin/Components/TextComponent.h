#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <memory>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Components/BaseComponent.h"

namespace dae
{
	class Font;
	class GameObject;
	class Texture2D;

	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> pFont, const SDL_Color& color = { 255, 255, 255, 255 });
		~TextComponent() = default;

		TextComponent(const TextComponent&)            = delete;
		TextComponent(TextComponent&&)                 = delete;
		TextComponent& operator=(const TextComponent&) = delete;
		TextComponent& operator=(TextComponent&&)      = delete;

		void Update(const float deltaTime) override;
		void Render() const override;

		void SetColor(const SDL_Color& color);
		void SetText(std::string& text);

	private:
		std::string m_text;
		std::shared_ptr<Font>	   m_font{ nullptr };
		std::shared_ptr<Texture2D> m_textTexture{ nullptr };
		SDL_Color m_color{ 255, 255, 255, 255 };
		bool m_needsUpdate{ true };
	};
}
#endif
