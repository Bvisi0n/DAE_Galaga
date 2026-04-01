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
        , m_observedEvent(observedEvent) {}

    void UIValueObserver::OnNotify(const GameEvent event)
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
