#include <cassert>	// assert
#include <iomanip>	// std::fixed, std::setprecision
#include <sstream>	// std::ostringstream
#include <string>	// std::string

#include "Components/FPSComponent.h"
#include "Components/TextComponent.h"
#include "GameObject.h"

dae::FPSComponent::FPSComponent(GameObject* pOwner, TextComponent* pText)
	: BaseComponent(pOwner)
	, m_pText(pText)
{
	assert(m_pText != nullptr && "FPSComponent requires a valid TextComponent reference.");
}

void dae::FPSComponent::Update(const float deltaTime)
{
	const float fps{ (deltaTime > 0.f) ? (1.f / deltaTime) : 0.f };

	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << fps << " FPS";
	std::string frame_string{ oss.str() };

	m_pText->SetText(frame_string);
}
