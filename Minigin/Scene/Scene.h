#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>
#include <vector>

#include "Minigin/Core/GameObject.h"

namespace dae::scene
{
	class Scene final
	{
	public:
		~Scene() = default;

		Scene(const Scene& other)			 = delete;
		Scene(Scene&& other)				 = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other)		 = delete;

		void Update(const float deltaTime);
		void Render() const;

		void Add(std::unique_ptr<core::GameObject> pObject);
		void Remove(const core::GameObject& object);
		void RemoveAll();

	private:
		friend class SceneManager;
		explicit Scene() = default;

		std::vector <std::unique_ptr<core::GameObject>> m_pObjects{};
	};
}
#endif
