#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>

namespace dae::core
{
	class GameObject;

	enum class ComponentState : uint8_t
	{
		Constructed = 0,
		Linked = 1,
		Ready = 2,
		Dead = 3
	};

	class Component
	{
	public:
		explicit Component( GameObject* pOwner ) noexcept
			: m_pOwner( pOwner )
		{}

		virtual ~Component() = default;

		Component( const Component& ) = delete;
		Component( Component&& ) = delete;
		Component& operator=( const Component& ) = delete;
		Component& operator=( Component&& ) = delete;

		ComponentState AdvanceState() noexcept
		{
			if ( m_state == ComponentState::Constructed )
			{
				InitializeLinkage();
				m_state = ComponentState::Linked;
			}
			else if ( m_state == ComponentState::Linked )
			{
				InitializeState();
				m_state = ComponentState::Ready;
			}

			return m_state;
		}

		virtual void Update( const float deltaTime ) = 0;

		void MarkForDeletion() noexcept
		{
			m_state = ComponentState::Dead;
		}

		[[nodiscard]] bool IsPendingDeletion() const noexcept
		{
			return m_state == ComponentState::Dead;
		}

		[[nodiscard]] ComponentState GetExecutionState() const noexcept
		{
			return m_state;
		}

	protected:
		virtual void InitializeLinkage() = 0;
		virtual void InitializeState() = 0;

		[[nodiscard]] GameObject* GetOwner() const noexcept
		{
			return m_pOwner;
		}

	private:
		GameObject* m_pOwner;
		ComponentState m_state{ ComponentState::Constructed };
	};
}
#endif
