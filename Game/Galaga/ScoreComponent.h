#ifndef BVI_SCORECOMPONENT_H
#define BVI_SCORECOMPONENT_H

#include "Minigin/Core/GameObject.h"
#include "Minigin/Events/ObservableComponent.h"

namespace bvi::galaga
{
	class ScoreComponent final : public dae::events::ObservableComponent
	{
	public:
		explicit ScoreComponent( dae::core::GameObject* owner );
		~ScoreComponent() override = default;

		ScoreComponent( const ScoreComponent& other ) = delete;
		ScoreComponent( ScoreComponent&& other ) = delete;
		ScoreComponent& operator=( const ScoreComponent& other ) = delete;
		ScoreComponent& operator=( ScoreComponent&& other ) = delete;

		void InitializeLinkage() noexcept override;
		void InitializeState() noexcept override;

		void Update( const float deltaTime ) noexcept override;

		void AddScore( const int score );
		[[nodiscard]] int GetScore() const;

	private:
		int m_score{};

		void NotifyScoreChanged();
	};
}
#endif
