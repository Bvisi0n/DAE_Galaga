#ifndef DAE_SCENEMANAGER_H
#define DAE_SCENEMANAGER_H

#include <memory>
#include <vector>

#include <Minigin/Utilities/Singleton.h>

namespace dae::scenes
{
	class Scene;

	class SceneManager final : public utils::Singleton<SceneManager>
	{
	public:
		SceneManager( const SceneManager& ) = delete;
		SceneManager( SceneManager&& ) = delete;
		SceneManager& operator=( const SceneManager& ) = delete;
		SceneManager& operator=( SceneManager&& ) = delete;

		~SceneManager();

		Scene& CreateScene();
		void RemoveAllScenes();

		void Update( float deltaTime );
		void Render();

		[[nodiscard]] Scene& GetActiveScene();
		[[nodiscard]] const Scene& GetActiveScene() const;

	private:
		friend class utils::Singleton<SceneManager>;

		// TODO dae_scenes - Using a vector to store a single scene.
		std::vector<std::unique_ptr<Scene>> m_scenes;
		size_t m_activeSceneIndex{ 0 };

		SceneManager();
	};
}
#endif
