#include <string>

#include "Components/TextureComponent.h"
#include "Components/TransformComponent.h"
#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#include "GameObject.h"
#include "Texture2D.h"

void dae::TextureComponent::Render() const
{
	if (m_pTexture != nullptr && GetOwner() != nullptr)
	{
		// DAEN: What if there is no TransformComponent?
		const auto& transform = GetOwner()->GetComponent<dae::TransformComponent>();
		const auto& pos = transform->GetPosition();
		const auto& scale = transform->GetScale();

		auto texture_size = m_pTexture->GetSize();
		float finalWidth = texture_size.x * scale.x;
		float finalHeight = texture_size.y * scale.y;

		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, finalWidth, finalHeight);
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
