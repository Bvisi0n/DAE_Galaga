#ifndef OBSERVABLECOMPONENT_H
#define OBSERVABLECOMPONENT_H

#include <vector>

#include "Components/Component.h"
#include "ObserverPattern/ISubject.h"

namespace dae
{
    class GameObject;
    class IObserver;

    class ObservableComponent : public Component, public ISubject
    {
    public:
        ObservableComponent(GameObject* pOwner)
            : Component(pOwner) {};
        virtual ~ObservableComponent() = default;

        virtual void Update(const float deltaTime) override = 0;
        virtual void Render() const override = 0;

        void AttachObserver(IObserver* observer) override;
        void DetachObserver(IObserver* observer) override;

    protected:
        void NotifyObservers() override;

    private:
        std::vector<IObserver*> m_pObservers;
    };
}
#endif
