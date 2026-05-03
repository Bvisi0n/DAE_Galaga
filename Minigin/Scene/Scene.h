#ifndef DAE_SCENE_H
#define DAE_SCENE_H

#include <memory>
#include <vector>

#include <Minigin/Core/CollisionSystem.h>

// C.4: Make a function a member only if it needs direct access to the representation of a class

namespace dae::core
{
	class GameObject;
}

namespace dae::scenes
{
	class SceneManager;

	void RemoveGameObject( core::GameObject& object );

	class Scene final
	{
	public:
		~Scene();

		Scene( const Scene& other ) = delete;
		Scene( Scene&& other ) = delete;
		Scene& operator=( const Scene& other ) = delete;
		Scene& operator=( Scene&& other ) = delete;

		void Initialize();
		void Update( float deltaTime );
		void Render() const;

		void AddGameObject( std::unique_ptr<core::GameObject> object );
		void CleanupGameObjects();
		void RemoveAllGameObjects();

		[[nodiscard]] core::CollisionSystem& GetCollisionSystem();

	private:
		friend class SceneManager;

		explicit Scene();

		void FlushPendingObjects();

		std::vector <std::unique_ptr<core::GameObject>> m_objects;
		std::vector<std::unique_ptr<core::GameObject>> m_pendingObjects;
		std::vector<std::unique_ptr<core::GameObject>> m_processingBuffer;
		core::CollisionSystem m_collisionSystem;
		bool m_isInitialized{ false };
	};
}
#endif
