#ifndef COMMAND_H
#define COMMAND_H

// TODO MINI: If all commands end up using a similar target context, consider moving the context pointer to the base Command class if it is allowed by the official pattern design.
	// https://gameprogrammingpatterns.com/command.html
	// https://refactoring.guru/design-patterns/command

namespace dae::input
{
	class Command
	{
	public:
		virtual ~Command() = default;

		virtual void Execute( const float deltaTime ) = 0;

		[[nodiscard]] virtual const void* GetTargetContext() const = 0;
	};
}
#endif
