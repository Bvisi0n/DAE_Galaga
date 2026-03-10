#include <string>

#include "Components/TextureComponent.h"
#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#include "GameObject.h"

void dae::TextureComponent::Render() const
{
	if (m_pTexture != nullptr && GetOwner() != nullptr)
	{
		const auto& pos = GetOwner()->GetGlobalPosition().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	if (m_filename != filename)
	{
		m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
		m_filename = filename;
	}
}
