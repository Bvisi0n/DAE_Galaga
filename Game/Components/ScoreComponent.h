#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "Minigin/Core/GameObject.h"
#include "Minigin/Events/ObservableComponent.h"

namespace bvi::components
{
	class ScoreComponent final : public dae::events::ObservableComponent
	{
	public:
		ScoreComponent( dae::core::GameObject* owner );
		virtual ~ScoreComponent();

		void InitializeLinkage() noexcept override
		{}

		void InitializeState() noexcept override;

		void Update( const float ) noexcept override
		{}

		void AddScore( const int score );
		[[nodiscard]] int GetScore() const;

	private:
		int m_score{};

		void NotifyScoreChanged();
	};
}
#endif
