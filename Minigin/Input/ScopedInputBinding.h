#ifndef DAE_SCOPEDINPUTBINDING_H
#define DAE_SCOPEDINPUTBINDING_H

#include <memory>
#include <type_traits>
#include <utility>
#include <variant>

#include "Minigin/Input/Gamepad.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Input/Keyboard.h"

// TODO dae_input - Split into .cpp/.h

namespace dae::input
{
	class ScopedInputBinding final
	{
	public:
		ScopedInputBinding( Keyboard::Key key, InputManager::KeyState state, std::unique_ptr<ICommand> command )
			: m_key{ InputManager::KeyboardBinding{key, state} }
		{
			InputManager::GetInstance().BindCommand( key, state, std::move( command ) );
		}

		ScopedInputBinding( Gamepad::Button button, InputManager::KeyState state, unsigned int controllerIndex, std::unique_ptr<ICommand> command )
			: m_key{ InputManager::ControllerBinding{controllerIndex, button, state} }
		{
			InputManager::GetInstance().BindCommand( button, state, std::move( command ), controllerIndex );
		}

		~ScopedInputBinding()
		{
			Unbind();
		}

		ScopedInputBinding( const ScopedInputBinding& ) = delete;
		ScopedInputBinding& operator=( const ScopedInputBinding& ) = delete;

		ScopedInputBinding( ScopedInputBinding&& other ) noexcept
			: m_key{ std::exchange( other.m_key, std::monostate{} ) }
		{}

		ScopedInputBinding& operator=( ScopedInputBinding&& other ) noexcept
		{
			if ( this != &other )
			{
				Unbind();
				m_key = std::exchange( other.m_key, std::monostate{} );
			}
			return *this;
		}

	private:
		InputManager::BindingKey m_key{ std::monostate{} };

		void Unbind()
		{
			std::visit( [] ( auto&& argument )
			{
				using T = std::decay_t<decltype( argument )>;

				if constexpr ( std::is_same_v<T, InputManager::KeyboardBinding> )
				{
					InputManager::GetInstance().UnbindCommand( argument.first, argument.second );
				}
				else if constexpr ( std::is_same_v<T, InputManager::ControllerBinding> )
				{
					InputManager::GetInstance().UnbindCommand( std::get<1>( argument ), std::get<2>( argument ), std::get<0>( argument ) );
				}
				// If (std::monostate) do nothing;

			}, m_key );
		}
	};
}
#endif
