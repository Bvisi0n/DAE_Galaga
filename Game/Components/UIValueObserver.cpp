#include <source_location>
#include <string>

#include "Game/Components/UIValueObserver.h"

#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/SDBMHash.h"

namespace bvi
{
	UIValueObserver::UIValueObserver(dae::core::GameObject* pOwner, dae::events::GameEvent observedEvent, Formatter formatter)
		: Component(pOwner)
		, m_Formatter(std::move(formatter))
		, m_observedEvent(observedEvent) {}

	void UIValueObserver::Initialize()
	{
		m_pText = GetOwner()->GetComponent<dae::graphics::TextComponent>();
		if (!m_pText)
		{
			assert(m_pText && "requires a TextComponent on the same GameObject.");
		}
	}

	void UIValueObserver::OnNotify(const dae::events::GameEvent event)
	{
		if (!m_pText)
		{
            // TODO N: This is triggered if object is initialized after the caller is.
                // 2 Step initialization? First sets up all the references, second sets all the initial values?
			assert(m_pText && "requires a TextComponent on the same GameObject.");
			return;
		}

		if (event.id == m_observedEvent.id)
		{
			if (m_pText && m_Formatter)
			{
				int value = std::get<int>(event.args[0]);

				m_pText->SetText(m_Formatter(value));
			}
		}
	}
}
