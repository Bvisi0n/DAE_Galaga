#ifndef BVI_GALAGASTATE_H
#define BVI_GALAGASTATE_H

#include "Minigin/Core/IAppState.h"

namespace bvi::galaga
{
	class GalagaState final : public dae::core::IAppState
	{
	public:
		GalagaState() = default;
		~GalagaState() override = default;

		GalagaState( const GalagaState& ) = delete;
		GalagaState( GalagaState&& ) = delete;
		GalagaState& operator=( const GalagaState& ) = delete;
		GalagaState& operator=( GalagaState&& ) = delete;

		void OnEnter() override;
		void OnExit() override;
		void Update( float deltaTime ) override;
	};
}
#endif