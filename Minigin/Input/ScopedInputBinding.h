#ifndef SCOPEDINPUTBINDING_H
#define SCOPEDINPUTBINDING_H

#include <memory>
#include <type_traits>
#include <utility>
#include <variant>

#include "Minigin/Input/Gamepad.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Input/Keyboard.h"

namespace dae::input
{
	class ScopedInputBinding final
	{
	public:
		ScopedInputBinding( Keyboard::Key key, InputManager::KeyState state, std::unique_ptr<Command> cmd )
			: m_key{ InputManager::KeyboardBinding{key, state} }
		{
			InputManager::GetInstance().BindCommand( key, state, std::move( cmd ) );
		}

		ScopedInputBinding( Gamepad::Button btn, InputManager::KeyState state, unsigned int controllerIndex, std::unique_ptr<Command> cmd )
			: m_key{ InputManager::ControllerBinding{controllerIndex, btn, state} }
		{
			InputManager::GetInstance().BindCommand( btn, state, std::move( cmd ), controllerIndex );
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
			std::visit( [] ( auto&& arg )
			{
				using T = std::decay_t<decltype( arg )>;

				if constexpr ( std::is_same_v<T, InputManager::KeyboardBinding> )
				{
					InputManager::GetInstance().UnbindCommand( arg.first, arg.second );
				}
				else if constexpr ( std::is_same_v<T, InputManager::ControllerBinding> )
				{
					InputManager::GetInstance().UnbindCommand( std::get<1>( arg ), std::get<2>( arg ), std::get<0>( arg ) );
				}
				// If (std::monostate) do nothing;

			}, m_key );
		}
	};
}
#endif
