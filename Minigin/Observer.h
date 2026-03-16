#ifndef OBSERVER_H
#define OBSERVER_H

namespace dae
{
    class Subject;

    class Observer
    {
        // https://gameprogrammingpatterns.com/observer.html
        // https://refactoring.guru/design-patterns/observer
        // https://refactoring.guru/design-patterns/observer/cpp/example
    public:
        virtual ~Observer();

        void RegisterSubject(Subject* subject);
        void DeRegisterSubject();

        virtual void OnNotify() = 0;

    private:
        Subject* m_pSubject = nullptr;
    };
}
#endif
