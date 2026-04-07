#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

#include <memory>
#include <string>

#include "Minigin/Core/Component.h"
#include "Minigin/Graphics/IRenderable.h"

namespace dae::graphics
{
	class Texture2D;

	// Renders an image using a Texture2D.
	class TextureComponent final : public core::Component, public IRenderable
	{
	public:
		using Component::Component;
		~TextureComponent() = default;

		TextureComponent( const TextureComponent& other ) = delete;
		TextureComponent( TextureComponent&& other ) noexcept = delete;
		TextureComponent& operator=( const TextureComponent& other ) = delete;
		TextureComponent& operator=( TextureComponent&& other ) noexcept = delete;

		void InitializeLinkage() override
		{}
		void InitializeState() override
		{}
		void Update( const float ) override
		{}
		void Render() const override;
		void SetTexture( const std::string& filename );

	private:
		std::string m_filename{};
		std::shared_ptr<Texture2D> m_texture{ nullptr };
	};
}
#endif
