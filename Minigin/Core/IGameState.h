#ifndef DAE_IGAMESTATE_H
#define DAE_IGAMESTATE_H

namespace dae::core
{
	class IGameState
	{
	public:
		IGameState() = default;
		virtual ~IGameState() = default;

		IGameState( const IGameState& ) = delete;
		IGameState( IGameState&& ) = delete;
		IGameState& operator=( const IGameState& ) = delete;
		IGameState& operator=( IGameState&& ) = delete;

		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;
		virtual void Update( float deltaTime ) = 0;
	};
}
#endif
