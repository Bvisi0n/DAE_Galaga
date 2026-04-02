#ifndef UIVALUEOBSERVER_H
#define UIVALUEOBSERVER_H

#include <functional>
#include <string>
#include <vector>

#include "Minigin/Core/Component.h"
#include "Minigin/Events/GameEvent.h"
#include "Minigin/Events/IObserver.h"

namespace dae::core
{
    class GameObject;
}

namespace dae::graphics
{
    class TextComponent;
}

namespace dae
{
    class UIValueObserver final : public core::Component, public events::IObserver
    {
    public:
        using Formatter = std::function<std::string(int)>;

        UIValueObserver(core::GameObject* pOwner, graphics::TextComponent* pText, events::GameEvent observedEvent, Formatter formatter);

        void Initialize() override {} // TODO L: Move m_pText assignment to Initialize() which should then extract the TextComponent from the owner GameObject.

        void OnNotify(const events::GameEvent event) override;

        void Update(const float) override {}

    private:
        Formatter m_Formatter;
        graphics::TextComponent* m_pText{ nullptr };
        events::GameEvent m_observedEvent; // TODO L: Does it have to be a whole GameEvent object?
    };
}
#endif
