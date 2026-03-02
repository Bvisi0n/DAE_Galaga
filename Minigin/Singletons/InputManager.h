#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Singletons/Singleton.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};
}
#endif
