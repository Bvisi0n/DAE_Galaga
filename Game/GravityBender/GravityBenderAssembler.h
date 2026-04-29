#ifndef BVI_GRAVITYBENDERASSEMBLER_H
#define BVI_GRAVITYBENDERASSEMBLER_H

#include <Minigin/Core/IAppState.h>
#include <Minigin/Scene/Scene.h>

namespace bvi::gravity_bender
{
	class GravityBenderAssembler final
	{
	public:
		GravityBenderAssembler() = delete;

		static void Assemble( dae::core::IAppState* /*stateMachine*/ );

	private:
		static void AssembleBackground( dae::scenes::Scene& scene );
		static void AssembleFPSCounter( dae::scenes::Scene& scene );
		static void AssembleInstructions( dae::scenes::Scene& scene );
		static void AssemblePlayer( dae::scenes::Scene& scene );
		static void AssembleSpawner( dae::scenes::Scene& scene );
		static void AssembleViewportBorder( dae::scenes::Scene& scene );
		static void EnableCollisions( dae::scenes::Scene& scene );
	};
}
#endif
