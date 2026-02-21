#ifndef FPSComponent_H
#define FPSComponent_H

#include <memory>

#include "BaseComponent.h"

namespace dae
{
	class Font;
	class GameObject;
	class TextComponent;

	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(GameObject* pOwner, std::shared_ptr<Font> pFont);
		~FPSComponent() = default;

		FPSComponent(const FPSComponent&)            = delete;
		FPSComponent(FPSComponent&&)                 = delete;
		FPSComponent& operator=(const FPSComponent&) = delete;
		FPSComponent& operator=(FPSComponent&&)      = delete;

		void Update(const float deltaTime) override;
		void Render() const override;

	private:
		std::unique_ptr<TextComponent> m_pText{ nullptr };
	};
}
#endif
