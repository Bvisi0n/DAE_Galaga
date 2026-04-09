#include <cassert>
#include <utility>
#include <variant>

#include "Game/Components/UIValueObserver.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Events/GameEvent.h"
#include "Minigin/Graphics/TextComponent.h"

namespace bvi::components
{
	UIValueObserver::UIValueObserver( dae::core::GameObject* owner, dae::events::GameEvent observedEvent, Formatter formatter )
		: Component( owner )
		, m_formatter( std::move( formatter ) )
		, m_observedEvent( observedEvent )
	{}

	void UIValueObserver::InitializeLinkage()
	{
		m_text = GetOwner()->GetComponent<dae::graphics::TextComponent>();
		if ( !m_text )
		{
			assert( m_text && "requires a TextComponent on the same GameObject." );
		}
	}

	void UIValueObserver::InitializeState()
	{}

	void UIValueObserver::Update( const float )
	{}

	void UIValueObserver::OnNotify( const dae::events::GameEvent event )
	{
		if ( !m_text )
		{
			assert( m_text && "requires a TextComponent on the same GameObject." );
			return;
		}

		if ( event.id == m_observedEvent.id )
		{
			if ( m_text && m_formatter )
			{
				int value = std::get<int>( event.args[ 0 ] );

				m_text->SetText( m_formatter( value ) );
			}
		}
	}
}
