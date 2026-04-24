#ifndef DAE_SCENEMANAGER_H
#define DAE_SCENEMANAGER_H

#include <memory>
#include <vector>

#include "Minigin/Scene/Scene.h"
#include "Minigin/Utilities/Singleton.h"

namespace dae::scenes
{
	class SceneManager final : public utils::Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();
		void RemoveAllScenes();

		void Update( const float deltaTime );
		void Render();

		[[nodiscard]] Scene& GetActiveScene();
		[[nodiscard]] const Scene& GetActiveScene() const;

	private:
		friend class utils::Singleton<SceneManager>;

		std::vector<std::unique_ptr<Scene>> m_scenes{};
		size_t m_activeSceneIndex{ 0 };

		SceneManager() = default;
		~SceneManager() = default;
	};
}
#endif
