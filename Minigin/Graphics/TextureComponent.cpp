#include <string>

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"

#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Graphics/Texture2D.h"
#include "Minigin/Graphics/TextureComponent.h"

#include "Minigin/Resources/ResourceManager.h"

namespace dae::graphics
{
	void TextureComponent::InitializeLinkage()
	{}

	void TextureComponent::InitializeState()
	{}

	void TextureComponent::Update( const float )
	{}

	void TextureComponent::Render() const
	{
		if ( m_texture != nullptr && GetOwner() != nullptr )
		{
			const auto& transform = GetOwner()->GetTransform();
			const auto& position = transform.GetWorldPosition();
			const auto& scale = transform.GetLocalScale();

			auto textureSize = m_texture->GetSize();
			float finalWidth = textureSize.x * scale.x;
			float finalHeight = textureSize.y * scale.y;

			Renderer::GetInstance().RenderTexture( *m_texture, position.x, position.y, finalWidth, finalHeight );
		}
	}

	void TextureComponent::SetTexture( const std::string& filename )
	{
		if ( m_filename != filename )
		{
			m_texture = resources::ResourceManager::GetInstance().LoadTexture( filename );
			m_filename = filename;
		}
	}
}
