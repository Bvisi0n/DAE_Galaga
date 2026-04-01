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

        void Initialize() noexcept override {}
        void Update(const float) noexcept override {}

        void AddScore(const int score);
        [[nodiscard]] int GetScore() const;

    private:
        int m_score{};
    };
}
#endif
