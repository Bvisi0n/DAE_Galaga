#ifndef BVI_GRAVITYBENDERSTATE_H
#define BVI_GRAVITYBENDERSTATE_H

#include <Minigin/Core/IAppState.h>

// TODO bvi_gravity_bender - Add a short-duration direction indicator for the spawner.

// TODO bvi_gravity_bender - Remove collisions between units.
// TODO bvi_gravity_bender - Units can only die by being absored by gravity wells.

// TODO bvi_gravity_bender - Start off with an example of gravity well catching some units. (If time allows)

namespace bvi::gravity_bender
{
	class GravityBenderState final : public dae::core::IAppState
	{
	public:
		GravityBenderState();
		~GravityBenderState() override;

		GravityBenderState( const GravityBenderState& ) = delete;
		GravityBenderState( GravityBenderState&& ) = delete;
		GravityBenderState& operator=( const GravityBenderState& ) = delete;
		GravityBenderState& operator=( GravityBenderState&& ) = delete;

		void OnEnter() override;
		void OnExit() override;
		void Update( float deltaTime ) override;
	};
}
#endif