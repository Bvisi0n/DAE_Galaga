#include "Game/Components/UIValueObserver.h"

#include "Minigin/Graphics/TextComponent.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/SDBMHash.h"

namespace dae
{
    UIValueObserver::UIValueObserver(core::GameObject* pOwner, TextComponent* pText, events::GameEvent observedEvent, Formatter formatter)
        : Component(pOwner)
        , m_Formatter(std::move(formatter))
        , m_pText(pText)
        , m_observedEvent(observedEvent) {}

    void UIValueObserver::OnNotify(const events::GameEvent event)
    {
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
