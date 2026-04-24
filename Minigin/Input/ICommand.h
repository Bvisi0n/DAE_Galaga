#ifndef ICOMMAND_H
#define ICOMMAND_H

// TODO dae_input - Commands should not store a context pointer, it should be passed in as an argument to the Execute function.
	// https://gameprogrammingpatterns.com/command.html
	// https://refactoring.guru/design-patterns/command

namespace dae::input
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;

		virtual void Execute( const float deltaTime ) = 0;

		[[nodiscard]] virtual const void* GetTargetContext() const = 0;
	};
}
#endif
