#include <string>

#include "Components/TextComponent.h"
#include "Components/UIValueObserver.h"
#include "GameObject.h"

namespace dae
{
    UIValueObserver::UIValueObserver(GameObject* pOwner, TextComponent* pText, GameEvent observedEvent, int initialValue)
        : Component(pOwner)
        , m_pText(pText)
        , m_observedEvent(observedEvent)
    {
        OnNotify(observedEvent, initialValue);
    }

    void UIValueObserver::OnNotify(GameEvent event, int value)
    {
        if (event == m_observedEvent)
        {
            if (m_pText)
            {
                std::string new_text = "Value: " + std::to_string(value);
                m_pText->SetText(new_text);
            }
        }
    }
}
