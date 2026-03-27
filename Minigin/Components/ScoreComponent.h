#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "Components/ObservableComponent.h"

namespace dae
{
    class ScoreComponent final : public ObservableComponent
    {
    public:
        ScoreComponent(GameObject* pOwner);
        virtual ~ScoreComponent() = default;

        void Update(float) override {}

        void AddScore(int score);
        int GetScore() const;

    private:
        int m_score{};
    };
}
#endif
