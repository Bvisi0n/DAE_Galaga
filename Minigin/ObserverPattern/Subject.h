#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>

#include "ObserverPattern/GameEvent.h"

namespace dae
{
    class Observer;

    class Subject
    {
        // https://gameprogrammingpatterns.com/observer.html
        // https://refactoring.guru/design-patterns/observer
        // https://refactoring.guru/design-patterns/observer/cpp/example

        // TODO N: This should be an interface, pure virtual with no data members. Read the material again.

    public:
        virtual ~Subject();

        virtual void AttachObserver(Observer* observer);
        virtual void DetachObserver(Observer* observer);

        virtual void NotifyObservers(GameEvent event, int value);

    protected:
        Subject() = default;

    private:
        std::vector<Observer*> m_pObservers;
    };
}
#endif
