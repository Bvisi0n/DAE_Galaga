#ifndef DAE_ICOMMAND_H
#define DAE_ICOMMAND_H

// TODO dae_input - Rework Commands
	// Commands should not store a context pointer, it should be passed in as an argument to the Execute function.

// https://gameprogrammingpatterns.com/command.html
// https://refactoring.guru/design-patterns/command

namespace dae::input
{
	class ICommand
	{
	public:
		ICommand() = default;
		virtual ~ICommand() = default;

		ICommand( const ICommand& ) = delete;
		ICommand( ICommand&& ) = delete;
		ICommand& operator=( const ICommand& ) = delete;
		ICommand& operator=( ICommand&& ) = delete;

		virtual void Execute( float deltaTime ) = 0;

		[[nodiscard]] virtual const void* GetTargetContext() const = 0;
	};
}
#endif
