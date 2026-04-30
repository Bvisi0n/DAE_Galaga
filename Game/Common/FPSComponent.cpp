#include <cassert>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>

#include "Game/Common/FPSComponent.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Graphics/TextComponent.h"

namespace bvi::common
{
	FPSComponent::FPSComponent( dae::core::GameObject* owner )
		: Component( owner )
	{}

	void FPSComponent::InitializeLinkage()
	{
		m_text = GetOwner()->GetComponent<dae::graphics::TextComponent>();
		if ( m_text == nullptr )
		{
			assert( m_text && "requires a TextComponent on the same GameObject." );
		}
	}

	void FPSComponent::InitializeState()
	{}

	void FPSComponent::Update( const float deltaTime )
	{
		if ( m_text == nullptr )
		{
			assert( m_text && "requires a TextComponent on the same GameObject." );
			return;
		}

		const float fps{ ( deltaTime > 0.F ) ? ( 1.F / deltaTime ) : 0.F };

		std::ostringstream oss;
		oss << std::fixed << std::setprecision( 1 ) << fps << " FPS";
		std::string frameString{ oss.str() };

		m_text->SetText( frameString );
	}
}
