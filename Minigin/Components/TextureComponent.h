#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

#include "BaseComponent.h"

namespace dae
{
	class Texture2D;
	class TextureComponent final : public BaseComponent
	{
	public:
		TextureComponent(GameObject* pOwner);
		~TextureComponent() override = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) noexcept = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) noexcept = delete;
		void Update(float deltaTime) override;
		void Render() const override;
		void SetTexture(const std::string& filename);
	private:
		std::shared_ptr<Texture2D> m_texture;
	};
}
#endif
