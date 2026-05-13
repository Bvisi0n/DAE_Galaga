#ifndef BVI_GRAVITYBENDERSTATE_H
#define BVI_GRAVITYBENDERSTATE_H

#include <Minigin/Core/IAppState.h>

// TODO bvi_gravity_bender - Redesign the spawner’s visual output to better communicate danger.
// TODO bvi_gravity_bender - Add a short-duration direction indicator for the spawner.
// TODO bvi_gravity_bender - Allow gravity wells to absorb units.
// TODO bvi_gravity_bender - Introduce an economy.
// TODO bvi_gravity_bender - Increase difficulty.

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