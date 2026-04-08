#ifndef IGAMESTATE_H
#define IGAMESTATE_H

// TODO L: Review state pattern properly.
	// https://gameprogrammingpatterns.com/state.html
	// https://refactoring.guru/design-patterns/state

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
		virtual void Update() = 0;
	};
}
#endif
