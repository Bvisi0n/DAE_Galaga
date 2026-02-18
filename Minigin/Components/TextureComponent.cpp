#include <string>

#include "Components/TextureComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

void dae::TextureComponent::Render() const
{
	const auto& pos = GetOwner()->GetTransform().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}
