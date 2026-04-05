#include <cassert>
#include <iomanip>
#include <sstream>
#include <string>

#include "Game/Components/FPSComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/TextComponent.h"

namespace bvi
{
	FPSComponent::FPSComponent(dae::core::GameObject* pOwner)
		: Component(pOwner) {}

	void FPSComponent::Initialize()
	{
		m_pText = GetOwner()->GetComponent<dae::graphics::TextComponent>();
		if (!m_pText)
		{
			assert(m_pText && "FPSComponent requires a TextComponent on the same GameObject.");
        }
	}

	void FPSComponent::Update(const float deltaTime)
	{
		if (!m_pText)
		{
			assert(m_pText && "FPSComponent requires a TextComponent on the same GameObject.");
			return;
		}

		const float fps{ (deltaTime > 0.f) ? (1.f / deltaTime) : 0.f };

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(1) << fps << " FPS";
		std::string frame_string{ oss.str() };

		m_pText->SetText(frame_string);
	}
}
