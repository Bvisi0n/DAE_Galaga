#ifndef UIVALUEOBSERVER_H
#define UIVALUEOBSERVER_H

#include "Components/Component.h"
#include "ObserverPattern/Observer.h"

namespace dae
{
    class GameObject;
    class TextComponent;

    class UIValueObserver final : public Component, public Observer
    {
    public:
        using Formatter = std::function<std::string(int)>;

        UIValueObserver(GameObject* pOwner, TextComponent* pText, GameEvent observedEvent, Formatter formatter, int initialValue = 0);

        void OnNotify(GameEvent event, int value) override;

        void Update(float) override {}
        void Render() const override {}

    private:
        Formatter m_Formatter;
        TextComponent* m_pText{ nullptr };
        GameEvent m_observedEvent;
    };
}
#endif
