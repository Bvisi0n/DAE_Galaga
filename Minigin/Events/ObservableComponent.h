#ifndef OBSERVABLECOMPONENT_H
#define OBSERVABLECOMPONENT_H

#include <vector>

#include "Minigin/Core/Component.h"
#include "Minigin/Events/ISubject.h"

namespace dae::core
{
    class GameObject;
}

namespace dae::events
{
    class IObserver;
    struct GameEvent;

    class ObservableComponent : public core::Component, public ISubject
    {
    public:
        ObservableComponent(core::GameObject* pOwner) noexcept
            : Component(pOwner) {};
        virtual ~ObservableComponent() = default;

        virtual void Initialize() override = 0;
        virtual void Update(const float deltaTime) override = 0;

        void AttachObserver(IObserver* observer) override;
        void DetachObserver(IObserver* observer) override;

    protected:
        void NotifyObservers(GameEvent event) const override;

    private:
        std::vector<IObserver*> m_pObservers;
    };
}
#endif
