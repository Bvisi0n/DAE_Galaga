#ifndef FPSComponent_H
#define FPSComponent_H

#include <memory>

#include "Minigin/Core/Component.h"

namespace dae::core
{
	class GameObject;
}

namespace dae::graphics
{
    class Font;
	class TextComponent;
}

namespace bvi
{
	// Displays the FPS in a given TextComponent.
	class FPSComponent final : public dae::core::Component
	{
	public:
		FPSComponent(dae::core::GameObject* pOwner, dae::graphics::TextComponent* pText);
		~FPSComponent() = default;

		FPSComponent(const FPSComponent&)            = delete;
		FPSComponent(FPSComponent&&)                 = delete;
		FPSComponent& operator=(const FPSComponent&) = delete;
		FPSComponent& operator=(FPSComponent&&)      = delete;

		void Initialize() override {} // TODO L: Should fetch m_pText from the owner.
		void Update(const float deltaTime) override;

	private:
		dae::graphics::TextComponent* m_pText{ nullptr };
	};
}
#endif
