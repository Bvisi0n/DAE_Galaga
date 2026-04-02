#include <algorithm>
#include <cassert>

#include "Minigin/Core/Transform.h"
#include "Minigin/Scene/Scene.h"

namespace dae::scene
{
	void Scene::Update(const float deltaTime)
	{
		for (auto& object : m_pObjects)
		{
			object->Update(deltaTime);
		}

		for (auto& object : m_pObjects)
		{
			if (!object->GetParent())
			{
				object->GetTransform().UpdateWorldMatrix(glm::mat4(1.0f));
			}
		}
	}

	void Scene::Render() const
	{
		// TODO N: Gameobjects marked for deletion should be removed before rendering.
		for (const auto& object : m_pObjects)
		{
			object->Render();
		}
	}

	void Scene::Add(std::unique_ptr<core::GameObject> pObject)
	{
		assert(pObject != nullptr && "Cannot add a null GameObject to the scene.");
		m_pObjects.emplace_back(std::move(pObject));
	}

	void Scene::Remove(const core::GameObject& object)
	{
		// TODO N: Use a flag to mark it for deletion, this function should be private and called in between Update() and Render(). The public variant should just mark it.
		m_pObjects.erase(
			std::remove_if(
				m_pObjects.begin(),
				m_pObjects.end(),
				[&object](const auto& ptr) { return ptr.get() == &object; }
			),
			m_pObjects.end()
		);
	}

	void Scene::RemoveAll()
	{
		m_pObjects.clear();
	}
}
