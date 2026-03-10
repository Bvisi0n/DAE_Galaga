#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <string>
#include <vector>

#include "Singletons/Singleton.h"
#include "Scene.h"

namespace dae
{
	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();

		void Update(const float deltaTime);
		void Render();

	private:
		friend class Singleton<SceneManager>;
		
		std::vector<std::unique_ptr<Scene>> m_pScenes{};

		SceneManager() = default;
	};
}
#endif
