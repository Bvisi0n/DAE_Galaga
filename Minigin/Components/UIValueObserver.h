#ifndef UIVALUEOBSERVER_H
#define UIVALUEOBSERVER_H

#include <functional>
#include <string>
#include <vector>

#include "Components/Component.h"
#include "ObserverPattern/IObserver.h"
#include "ObserverPattern/GameEvent.h"

namespace dae
{
    class GameObject;
    class TextComponent;

    class UIValueObserver final : public Component, public IObserver
    {
    public:
        using Formatter = std::function<std::string(int)>;

        UIValueObserver(GameObject* pOwner, TextComponent* pText, GameEvent observedEvent, Formatter formatter);

        void OnNotify(const GameEvent event) override;

        void Update(float) override {}
        void Render() const override {}

    private:
        Formatter m_Formatter;
        TextComponent* m_pText{ nullptr };
        GameEvent m_observedEvent;
    };
}
#endif
