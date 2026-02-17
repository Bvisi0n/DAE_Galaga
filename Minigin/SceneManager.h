#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <string>
#include <vector>

#include "Scene.h"
#include "Singleton.h"

// Forward declarations
class Scene;

namespace dae
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();

		void FixedUpdate(const float deltaTime);
		void Update(const float deltaTime);
		void Render();

	private:
		friend class Singleton<SceneManager>;
		
		std::vector<std::unique_ptr<Scene>> m_scenes{};

		SceneManager() = default;
	};
}
#endif
