#ifndef COMMAND_H
#define COMMAND_H

namespace dae::input
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute( const float deltaTime ) = 0;
	};
}
#endif
