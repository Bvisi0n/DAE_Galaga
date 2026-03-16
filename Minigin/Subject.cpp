#include "Subject.h"

namespace dae
{
    Subject::~Subject()
    {
        for (auto* observer : m_pObservers)
        {
            observer->DeRegisterSubject();
        }
    }

    void Subject::Attach(dae::Observer* observer)
    {
        if (observer == nullptr)
        {
            return;
        }
        else if (std::ranges::find(m_pObservers, observer) == m_pObservers.end())
        {
            m_pObservers.push_back(observer);
            observer->RegisterSubject(this);
        }
    }

    void Subject::Detach(dae::Observer* observer)
    {
        std::erase(m_pObservers, observer);
    }
}
