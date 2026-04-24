#ifndef DAE_TEXTCOMPONENT_H
#define DAE_TEXTCOMPONENT_H

#include <memory>
#include <string>

#include <SDL3/SDL_pixels.h>

#include "Minigin/Core/Component.h"
#include "Minigin/Graphics/IRenderable.h"

namespace dae::core
{
	class GameObject;
}

namespace dae::graphics
{
	class Font;
	class Texture2D;

	class TextComponent final : public core::Component, public IRenderable
	{
	public:
		explicit TextComponent( core::GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 } );
		~TextComponent() override = default;

		TextComponent( const TextComponent& ) = delete;
		TextComponent( TextComponent&& ) = delete;
		TextComponent& operator=( const TextComponent& ) = delete;
		TextComponent& operator=( TextComponent&& ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float deltaTime ) override;
		void Render() const override;

		void SetColor( const SDL_Color& color );
		void SetText( const std::string& text );

	private:
		std::string m_text;
		std::shared_ptr<Font> m_font{ nullptr };
		std::shared_ptr<Texture2D> m_texture{ nullptr };
		SDL_Color m_color{ 255, 255, 255, 255 };
		bool m_needsUpdate{ true };
	};
}
#endif
