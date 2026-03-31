#include <string>

#include "Components/TextureComponent.h"
#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "Transform.h"

void dae::TextureComponent::Render() const
{
	if (m_pTexture != nullptr && GetOwner() != nullptr)
	{
		const auto& transform = GetOwner()->GetTransform();
		const auto& position = transform.GetWorldPosition();
		const auto& scale = transform.GetLocalScale();

		auto texture_size = m_pTexture->GetSize();
		float final_width = texture_size.x * scale.x;
		float final_height = texture_size.y * scale.y;

		Renderer::GetInstance().RenderTexture(*m_pTexture, position.x, position.y, final_width, final_height);
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
