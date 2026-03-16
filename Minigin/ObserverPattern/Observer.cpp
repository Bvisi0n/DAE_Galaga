#include "ObserverPattern/Observer.h"
#include "ObserverPattern/Subject.h"

namespace dae
{
    Observer::~Observer()
    {
        DeRegisterSubject();
    }

    void Observer::RegisterSubject(Subject* subject)
    {
        m_pSubject = subject;
    }

    void Observer::DeRegisterSubject()
    {
        if (m_pSubject)
        {
            m_pSubject->DetachObserver(this);
            m_pSubject = nullptr;
        }
    }
}
