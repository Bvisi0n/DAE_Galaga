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
		// DAEN: Scene should own all components and store them in cache efficient std::vectors.
		// Custom components could for now be stored in a shared vector of unique_ptr<BaseComponent>, more research is needed for those cases.
		// GameObjects should be renamed to GameEntity and should only be used to group components together.
		// GameEntities should only have an ID and scene graph logic.
		// You will need a way to link an Entity ID to an index in these vectors. A "Sparse Set" or a simple lookup table.
		// Ensure your Component structs are sized to multiples of 64 bytes to prevent "False Sharing."
		// Ensure the Global Transform calculation is done in a single pass over the TransformComponent vector to maintain linear memory access.

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
