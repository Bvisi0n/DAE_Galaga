#include "Minigin/Scene/Scene.h"
#include "Minigin/Scene/SceneManager.h"

namespace dae::scene
{
	Scene& SceneManager::CreateScene()
	{
		m_pScenes.emplace_back(new Scene());
		return *m_pScenes.back();
	}

	void SceneManager::Update(const float deltaTime)
	{
		for (auto& scene : m_pScenes)
		{
			scene->Update(deltaTime);
		}
	}

	void SceneManager::Render()
	{
		for (const auto& scene : m_pScenes)
		{
			scene->Render();
		}
	}
}
