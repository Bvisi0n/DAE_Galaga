#ifndef BVI_PUSHSTATECOMMAND_H
#define BVI_PUSHSTATECOMMAND_H

#include <memory>
#include <type_traits>

#include "Minigin/Core/AppStateManager.h"
#include "Minigin/Core/IAppState.h"
#include "Minigin/Input/ICommand.h"

// TODO bvi_common - Commands should not store a context pointer, it should be passed in as an argument to the Execute function.

namespace bvi::common
{
	template <typename T>
	concept IsGameState = std::is_base_of_v<dae::core::IAppState, T>;

	template <IsGameState StateType>
	class PushStateCommand final : public dae::input::ICommand
	{
	public:
		explicit PushStateCommand( const void* targetContext ) noexcept
			: m_targetContext{ targetContext }
		{}

		void Execute( const float /*deltaTime*/ ) override
		{
			dae::core::AppStateManager::GetInstance().ChangeState( std::make_unique<StateType>() );
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