#include <glm/ext/vector_float3.hpp>

#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Core/Transform.h>

#include <Game/GravityBender/GravityBenderBlueprints.h>
#include <Game/GravityBender/ScreenWrapComponent.h>

namespace bvi::gravity_bender
{
	ScreenWrapComponent::ScreenWrapComponent( dae::core::GameObject* owner, float spriteOffset )
		: dae::core::Component( owner )
		, m_spriteOffset{ spriteOffset }
	{}

	ScreenWrapComponent::~ScreenWrapComponent() = default;

	void ScreenWrapComponent::InitializeLinkage()
	{}

	void ScreenWrapComponent::InitializeState()
	{}

	void ScreenWrapComponent::Update( const float /*deltaTime*/ )
	{
		constexpr const auto& viewportConfig = bvi::gravity_bender::config::Config.viewport;

		auto& transform = GetOwner()->GetTransform();
		glm::vec3 currentPos = transform.GetLocalPosition();
		bool hasWrapped{ false };

		if ( currentPos.x < -m_spriteOffset )
		{
			currentPos.x += ( viewportConfig.width + ( m_spriteOffset * 2.F ) );
			hasWrapped = true;
		}
		else if ( currentPos.x > viewportConfig.width + m_spriteOffset )
		{
			currentPos.x -= ( viewportConfig.width + ( m_spriteOffset * 2.F ) );
			hasWrapped = true;
		}

		if ( currentPos.y < -m_spriteOffset )
		{
			currentPos.y += ( viewportConfig.height + ( m_spriteOffset * 2.F ) );
			hasWrapped = true;
		}
		else if ( currentPos.y > viewportConfig.height + m_spriteOffset )
		{
			currentPos.y -= ( viewportConfig.height + ( m_spriteOffset * 2.F ) );
			hasWrapped = true;
		}

		if ( hasWrapped )
		{
			transform.SetLocalPosition( currentPos );
		}
	}
}
