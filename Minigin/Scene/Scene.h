#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include "Minigin/Core/CollisionSystem.h"
#include "Minigin/Core/GameObject.h"

namespace dae::scene
{
	class Scene final
	{
	public:
		~Scene() = default;

		Scene( const Scene& other ) = delete;
		Scene( Scene&& other ) = delete;
		Scene& operator=( const Scene& other ) = delete;
		Scene& operator=( Scene&& other ) = delete;

		void Initialize();
		void Update( const float deltaTime );
		void Render() const;

		void AddGameObject( std::unique_ptr<core::GameObject> object );
		void RemoveGameObject( core::GameObject& object );
		void CleanupGameObjects();
		void RemoveAllGameObjects();

		[[nodiscard]] core::CollisionSystem& GetCollisionSystem();

	private:
		friend class SceneManager;

		explicit Scene() = default;

		void FlushPendingObjects();

		std::vector <std::unique_ptr<core::GameObject>> m_objects{};
		std::vector<std::unique_ptr<core::GameObject>> m_pendingObjects{};
		core::CollisionSystem m_collisionSystem{};
		bool m_isInitialized{ false };
	};
}
#endif
