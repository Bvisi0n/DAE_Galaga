#include "Components/TextComponent.h"
#include "Components/UIValueObserver.h"
#include "GameObject.h"
#include "SDBMHash.h"

namespace dae
{
    UIValueObserver::UIValueObserver(GameObject* pOwner, TextComponent* pText, GameEvent observedEvent, Formatter formatter)
        : Component(pOwner)
        , m_Formatter(std::move(formatter))
        , m_pText(pText)
        , m_observedEvent(observedEvent)
    {
        OnNotify(observedEvent);
    }

    void UIValueObserver::OnNotify(const GameEvent event)
    {
        if (event.id == m_observedEvent.id)
        {
            if (m_pText && m_Formatter)
            {
                int value = 0; // TODO H: Continue here. Should pull data from observed subject. Just have OnNotify accept std::variant, maybe use a getter lambda passed with constructor?
                m_pText->SetText(m_Formatter(value));
            }
        }
    }
}
