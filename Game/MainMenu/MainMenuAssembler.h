#ifndef BVI_MAINMENUASSEMBLER_H
#define BVI_MAINMENUASSEMBLER_H

#include <Minigin/Scene/Scene.h>

namespace dae::core
{
	class IAppState;
}

namespace bvi::main_menu
{
	class MainMenuAssembler final
	{
	public:
		MainMenuAssembler() = delete;

		static void Assemble( dae::core::IAppState* stateMachine );

	private:
		static void AssembleBackground( dae::scenes::Scene& scene );
		static void AssembleLogo( dae::scenes::Scene& scene );
		static void AssembleFPSCounter( dae::scenes::Scene& scene );
		static void AssembleGalagaUI( dae::scenes::Scene& scene, dae::core::IAppState* stateMachine );
		static void AssembleGravityBenderUI( dae::scenes::Scene& scene, dae::core::IAppState* stateMachine );
	};
}
#endif
