#ifndef BVI_GALAGAASSEMBLER_H
#define BVI_GALAGAASSEMBLER_H

#include <Minigin/Core/IAppState.h>
#include <Minigin/Scene/Scene.h>

namespace bvi::galaga
{
	class GalagaMenuAssembler final
	{
	public:
		GalagaMenuAssembler() = delete;

		static void Assemble( dae::core::IAppState* stateMachine );

	private:
		static void AssembleBackground( dae::scenes::Scene& scene );
		static void AssembleBackToMainMenuUI( dae::scenes::Scene& scene, dae::core::IAppState* stateMachine );
		static void AssembleFPSCounter( dae::scenes::Scene& scene );
	};
}
#endif
