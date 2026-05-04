#ifndef BVI_GALAGASTATE_H
#define BVI_GALAGASTATE_H

#include <Minigin/Core/IAppState.h>

namespace bvi::galaga
{
	class GalagaMenuState final : public dae::core::IAppState
	{
	public:
		GalagaMenuState() = default;
		~GalagaMenuState() override = default;

		GalagaMenuState( const GalagaMenuState& ) = delete;
		GalagaMenuState( GalagaMenuState&& ) = delete;
		GalagaMenuState& operator=( const GalagaMenuState& ) = delete;
		GalagaMenuState& operator=( GalagaMenuState&& ) = delete;

		void OnEnter() override;
		void OnExit() override;
		void Update( float deltaTime ) override;
	};
}
#endif