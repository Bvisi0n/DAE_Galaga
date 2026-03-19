#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>
#include <vector>

#include "GameObject.h"

namespace dae
{
	class Scene final
	{
		// DAEL: The ECS dream, can it be done? I really want to achieve it!
		// Back in 2024 around the time of my first readthrough of Gameplay Programming Patterns, I learned of it's existence and was immediately intrigued by the idea of DOD and it's insane perfomance but I still don't really understand how to actually do it...

	public:
		~Scene() = default;

		Scene(const Scene& other)			 = delete;
		Scene(Scene&& other)				 = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other)		 = delete;

		void Update(const float deltaTime);
		void Render() const;

		void Add(std::unique_ptr<GameObject> pObject);
		void Remove(const GameObject& object);
		void RemoveAll();

	private:
		friend class SceneManager;
		explicit Scene() = default;

		std::vector <std::unique_ptr<GameObject>> m_pObjects{};
	};
}
#endif
