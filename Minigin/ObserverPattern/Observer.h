#ifndef OBSERVER_H
#define OBSERVER_H

#include "ObserverPattern/GameEvent.h"

namespace dae
{
    class Subject;

    class Observer
    {
        // https://gameprogrammingpatterns.com/observer.html
        // https://refactoring.guru/design-patterns/observer
        // https://refactoring.guru/design-patterns/observer/cpp/example

        // DAEN: This should be an interface, pure virtual with no data members. Read the material again.
        // DAEN: Current implementation only allows an observer to observe a single subject, a problem for achievements.

    public:
        virtual ~Observer();

        void RegisterSubject(Subject* subject);
        void DeRegisterSubject();

        virtual void OnNotify(GameEvent event, int value) = 0;

    private:
        Subject* m_pSubject = nullptr;
    };
}
#endif
