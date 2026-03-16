#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "Components/Component.h"
#include "ObserverPattern/Subject.h"

namespace dae
{
    class ScoreComponent final : public Component, public Subject
    {
    public:
        ScoreComponent(GameObject* pOwner);
        virtual ~ScoreComponent() = default;

        void Update(float) override {}
        void Render() const override {}

        void AddScore(int score);
        int GetScore() const;

    private:
        int m_score{};
    };
}
#endif
