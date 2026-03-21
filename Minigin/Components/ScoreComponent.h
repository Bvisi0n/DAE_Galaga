#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include <vector>

#include "Components/Component.h"
#include "ObserverPattern/ISubject.h"

namespace dae
{
    class ScoreComponent final : public Component, public ISubject
    {
    public:
        ScoreComponent(GameObject* pOwner);
        virtual ~ScoreComponent() = default;

        void Update(float) override {}
        void Render() const override {}

        void AttachObserver(IObserver* observer) override;
        void DetachObserver(IObserver* observer) override;
        void NotifyObservers() override;

        void AddScore(int score);
        int GetScore() const;

    private:
        std::vector<IObserver*> m_pObservers;
        int m_score{};
    };
}
#endif
