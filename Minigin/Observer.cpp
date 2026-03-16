#include "Observer.h"
#include "Subject.h"

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
        m_pSubject->Detach(this);
        m_pSubject = nullptr;
    }
}
