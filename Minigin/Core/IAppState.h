#ifndef DAE_IAPPSTATE_H
#define DAE_IAPPSTATE_H

namespace dae::core
{
	class IAppState
	{
	public:
		IAppState() = default;
		virtual ~IAppState() = default;

		IAppState( const IAppState& ) = delete;
		IAppState( IAppState&& ) = delete;
		IAppState& operator=( const IAppState& ) = delete;
		IAppState& operator=( IAppState&& ) = delete;

		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;
		virtual void Update( float deltaTime ) = 0;
	};
}
#endif
