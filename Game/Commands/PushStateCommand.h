#ifndef PUSHSTATECOMMAND_H
#define PUSHSTATECOMMAND_H

#include <concepts>
#include <memory>

#include "Minigin/Core/AppStateManager.h"
#include "Minigin/Core/IGameState.h"
#include "Minigin/Input/Command.h"



namespace bvi::commands
{
	template <typename T>
	concept IsGameState = std::is_base_of_v<dae::core::IGameState, T>;

	template <IsGameState StateType>
	class PushStateCommand final : public dae::input::Command
	{
	public:
		explicit PushStateCommand( const void* targetContext ) noexcept
			: m_targetContext{ targetContext }
		{}

		void Execute( const float ) override
		{
			dae::core::AppStateManager::GetInstance().PushState( std::make_unique<StateType>() );
		}

		[[nodiscard]] const void* GetTargetContext() const override
		{
			return m_targetContext;
		}

	private:
		const void* m_targetContext;
	};
}
#endif