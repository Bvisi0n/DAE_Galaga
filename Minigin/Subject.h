#ifndef SUBJECT_H
#define SUBJECT_H

#include <algorithm>
#include <memory>
#include <vector>

#include "Observer.h"

namespace dae
{
    class Subject
    {
        // https://gameprogrammingpatterns.com/observer.html
        // https://refactoring.guru/design-patterns/observer
        // https://refactoring.guru/design-patterns/observer/cpp/example
    public:
        virtual ~Subject();

        virtual void Attach(dae::Observer* observer);
        virtual void Detach(dae::Observer* observer);

        virtual void Notify() = 0;

    protected:
        Subject() = default;

    private:
        std::vector<dae::Observer*> m_pObservers;
    };
}
#endif
