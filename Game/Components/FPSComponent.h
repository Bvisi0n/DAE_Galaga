#ifndef FPSComponent_H
#define FPSComponent_H

#include <memory>

#include "Minigin/Core/Component.h"

namespace dae
{
	class Font;
	class GameObject;
	class TextComponent;

	// Displays the FPS in a given TextComponent.
	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* pOwner, TextComponent* pText);
		~FPSComponent() = default;

		FPSComponent(const FPSComponent&)            = delete;
		FPSComponent(FPSComponent&&)                 = delete;
		FPSComponent& operator=(const FPSComponent&) = delete;
		FPSComponent& operator=(FPSComponent&&)      = delete;

		void Initialize() override {} // TODO L: Should fetch m_pText from the owner.
		void Update(const float deltaTime) override;

	private:
		TextComponent* m_pText{ nullptr };
	};
}
#endif
