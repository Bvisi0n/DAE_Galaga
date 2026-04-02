#include <string>

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"

#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Graphics/TextureComponent.h"
#include "Minigin/Graphics/Texture2D.h"

#include "Minigin/Resources/ResourceManager.h"

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
