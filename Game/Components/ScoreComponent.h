#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "Minigin/Events/ObservableComponent.h"

namespace bvi
{
	class ScoreComponent final : public dae::events::ObservableComponent
	{
	public:
		ScoreComponent(dae::core::GameObject* pOwner);
		virtual ~ScoreComponent() = default;

		void InitializeLinkage() noexcept override {}
		void InitializeState() noexcept override;
		void Update(const float) noexcept override {}

		void AddScore(const int score);
		[[nodiscard]] int GetScore() const;

	private:
		int m_score{};

		void NotifyScoreChanged();
	};
}
#endif
