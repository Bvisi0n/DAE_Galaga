#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

#include <memory>
#include <string>

#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	class TextureComponent final : public BaseComponent
	{
	public:
		using BaseComponent::BaseComponent;
		virtual ~TextureComponent() = default;

		TextureComponent(const TextureComponent& other)				   = delete;
		TextureComponent(TextureComponent&& other) noexcept			   = delete;
		TextureComponent& operator=(const TextureComponent& other)	   = delete;
		TextureComponent& operator=(TextureComponent&& other) noexcept = delete;

		void Update(const float) override {};
		void Render() const override;
		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_texture{ nullptr };
	};
}
#endif
