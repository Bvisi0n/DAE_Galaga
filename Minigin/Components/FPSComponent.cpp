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

void dae::FPSComponent::Update([[maybe_unused]] const float deltaTime)
{
	std::string frameString{ std::to_string(1 / deltaTime) };

	m_pText->SetText(frameString);

	m_pText->Update(deltaTime);
}

void dae::FPSComponent::Render() const
{
	m_pText->Render();
}
