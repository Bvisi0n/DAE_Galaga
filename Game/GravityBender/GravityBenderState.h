#ifndef BVI_GRAVITYBENDERSTATE_H
#define BVI_GRAVITYBENDERSTATE_H

#include <Minigin/Core/IAppState.h>

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