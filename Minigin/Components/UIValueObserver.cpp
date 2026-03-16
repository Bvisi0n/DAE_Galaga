#include <functional>
#include <string>

#include "Components/TextComponent.h"
#include "Components/UIValueObserver.h"
#include "GameObject.h"

namespace dae
{
    UIValueObserver::UIValueObserver(GameObject* pOwner, TextComponent* pText, GameEvent observedEvent, Formatter formatter, int initialValue)
        : Component(pOwner)
        , m_Formatter(std::move(formatter))
        , m_pText(pText)
        , m_observedEvent(observedEvent)
    {
        OnNotify(observedEvent, initialValue);
    }

    void UIValueObserver::OnNotify(GameEvent event, int value)
    {
        if (event == m_observedEvent)
        {
            if (m_pText && m_Formatter)
            {
                m_pText->SetText(m_Formatter(value));
            }
        }
    }
}
