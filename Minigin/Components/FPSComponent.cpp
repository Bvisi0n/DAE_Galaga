#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

#include "Components/FPSComponent.h"
#include "Components/TextComponent.h"
#include "Font.h"
#include "GameObject.h"

dae::FPSComponent::FPSComponent(GameObject* pOwner, std::shared_ptr<Font> pFont)
	: BaseComponent(pOwner)
{
	m_pText = std::make_unique<TextComponent>(GetOwner(), "", std::move(pFont));
}

void dae::FPSComponent::Update(const float deltaTime)
{
	const float fps{ (deltaTime > 0.f) ? (1.f / deltaTime) : 0.f };

	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << fps << " FPS";
	std::string frameString{ oss.str() };

	m_pText->SetText(frameString);

	m_pText->Update(deltaTime);
}

void dae::FPSComponent::Render() const
{
	m_pText->Render();
}
