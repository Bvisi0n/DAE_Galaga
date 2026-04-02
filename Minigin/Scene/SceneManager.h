#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <string>
#include <vector>

#include "Minigin/Scene/Scene.h"
#include "Minigin/Utilities/Singleton.h"

namespace dae::scene
{
	class Scene;

	class SceneManager final : public utils::Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();

		void Update(const float deltaTime);
		void Render();

	private:
		friend class utils::Singleton<SceneManager>;
		
		std::vector<std::unique_ptr<Scene>> m_pScenes{};

		SceneManager() = default;
		virtual ~SceneManager() = default;
	};
}
#endif
