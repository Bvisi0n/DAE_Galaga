#include <cassert>
#include <iomanip>
#include <sstream>
#include <string>

#include "Game/Components/FPSComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/TextComponent.h"

namespace dae
{
	FPSComponent::FPSComponent(GameObject* pOwner, TextComponent* pText)
		: Component(pOwner)
		, m_pText(pText)
	{
		assert(m_pText != nullptr && "FPSComponent requires a valid TextComponent reference.");
	}

	void FPSComponent::Update(const float deltaTime)
	{
		const float fps{ (deltaTime > 0.f) ? (1.f / deltaTime) : 0.f };

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(1) << fps << " FPS";
		std::string frame_string{ oss.str() };

		m_pText->SetText(frame_string);
	}
}
