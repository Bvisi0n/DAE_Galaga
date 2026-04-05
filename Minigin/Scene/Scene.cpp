#include <algorithm>
#include <cassert>

#include "Minigin/Core/Transform.h"
#include "Minigin/Scene/Scene.h"

namespace dae::scene
{
	void Scene::Initialize()
	{
		for (auto& object : m_pObjects)
		{
			object->InitializeLinkage();
		}

		for (auto& object : m_pObjects)
		{
			object->InitializeState();
		}
	}

	void Scene::Update(const float deltaTime)
	{
		for (auto& object : m_pObjects)
		{
			if (!object->IsPendingDeletion())
			{
				object->Update(deltaTime);
			}
		}

		for (auto& object : m_pObjects)
		{
			if (!object->IsPendingDeletion() && !object->GetParent())
			{
				object->GetTransform().UpdateWorldMatrix(glm::mat4(1.0f));
			}
		}

		CleanupGameObjects();
	}

	void Scene::Render() const
	{
		for (const auto& object : m_pObjects)
		{
			object->Render();
		}
	}

	void Scene::AddGameObject(std::unique_ptr<core::GameObject> pObject)
	{
		assert(pObject != nullptr && "Cannot add a null GameObject to the scene.");
		m_pObjects.emplace_back(std::move(pObject));
	}

	void Scene::RemoveGameObject(core::GameObject& object)
	{
		object.MarkForDeletion();
	}

	void Scene::CleanupGameObjects()
	{
		std::erase_if(m_pObjects,
			[](const auto& pObj)
			{
				return pObj->IsPendingDeletion();
			});
	}

	void Scene::RemoveAllGameObjects()
	{
		m_pObjects.clear();
	}
}
