#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

#include <memory>
#include <string>

#include "Components/Component.h"
#include "Components/IRenderable.h"

namespace dae
{
	class Texture2D;

	// Renders an image using a Texture2D.
    class TextureComponent final : public Component, public IRenderable
	{
	public:
		using Component::Component;
		~TextureComponent() = default;

		TextureComponent(const TextureComponent& other)				   = delete;
		TextureComponent(TextureComponent&& other) noexcept			   = delete;
		TextureComponent& operator=(const TextureComponent& other)	   = delete;
		TextureComponent& operator=(TextureComponent&& other) noexcept = delete;

		void Initialize() override {}
		void Update(const float) override {}
		void Render() const override;
		void SetTexture(const std::string& filename);

	private:
		std::string m_filename{};
		std::shared_ptr<Texture2D> m_pTexture{ nullptr };
	};
}
#endif
