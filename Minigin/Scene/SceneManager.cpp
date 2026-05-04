#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/SceneManager.h>

namespace dae::scenes
{
	Scene& SceneManager::CreateScene()
	{
		m_scenes.emplace_back( new Scene() );

		if ( m_scenes.size() == 1 )
		{
			m_activeSceneIndex = 0;
		}

		return *m_scenes.back();
	}

	void SceneManager::RemoveAllScenes()
	{
		m_scenes.clear();
	}

	void SceneManager::Update( const float deltaTime )
	{
		for ( auto& scene : m_scenes )
		{
			scene->Update( deltaTime );
		}
	}

	void SceneManager::Render()
	{
		for ( const auto& scene : m_scenes )
		{
			scene->Render();
		}
	}

	Scene& SceneManager::GetActiveScene()
	{
		return *m_scenes.at( m_activeSceneIndex );
	}

	const Scene& SceneManager::GetActiveScene() const
	{
		return *m_scenes.at( m_activeSceneIndex );
	}
}
