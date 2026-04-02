#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "Minigin/Events/ObservableComponent.h"

namespace dae
{
    class ScoreComponent final : public events::ObservableComponent
    {
    public:
        ScoreComponent(core::GameObject* pOwner);
        virtual ~ScoreComponent() = default;

        void Initialize() noexcept override;
        void Update(const float) noexcept override {}

        void AddScore(const int score);
        [[nodiscard]] int GetScore() const;

    private:
        int m_score{};

        void NotifyScoreChanged() const;
    };
}
#endif
