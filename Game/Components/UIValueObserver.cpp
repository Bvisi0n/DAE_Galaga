#include <cassert>
#include <utility>
#include <variant>

#include "Game/Components/UIValueObserver.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Events/GameEvent.h"
#include "Minigin/Graphics/TextComponent.h"

namespace bvi
{
	UIValueObserver::UIValueObserver( dae::core::GameObject* pOwner, dae::events::GameEvent observedEvent, Formatter formatter )
		: Component( pOwner )
		, m_Formatter( std::move( formatter ) )
		, m_observedEvent( observedEvent )
	{}

	void UIValueObserver::InitializeLinkage()
	{
		m_pText = GetOwner()->GetComponent<dae::graphics::TextComponent>();
		if ( !m_pText )
		{
			assert( m_pText && "requires a TextComponent on the same GameObject." );
		}
	}

	void UIValueObserver::OnNotify( const dae::events::GameEvent event )
	{
		if ( !m_pText )
		{
			assert( m_pText && "requires a TextComponent on the same GameObject." );
			return;
		}

		if ( event.id == m_observedEvent.id )
		{
			if ( m_pText && m_Formatter )
			{
				int value = std::get<int>( event.args[ 0 ] );

				m_pText->SetText( m_Formatter( value ) );
			}
		}
	}
}
