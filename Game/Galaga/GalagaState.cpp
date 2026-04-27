#include "Game/Galaga/GalagaAssembler.h"
#include "Game/Galaga/GalagaState.h"

#include "Minigin/Scene/SceneManager.h"

namespace bvi::galaga
{
	void GalagaState::OnEnter()
	{
		GalagaAssembler::Assemble( this );
	}

	void GalagaState::OnExit()
	{
		dae::scenes::SceneManager::GetInstance().RemoveAllScenes();
	}

	void GalagaState::Update( float )
	{}
}