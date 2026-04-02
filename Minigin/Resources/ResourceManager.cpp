#include <stdexcept>

#include <SDL3_ttf/SDL_ttf.h>

#include "Minigin/Graphics/Font.h"
#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Graphics/Texture2D.h"
#include "Minigin/Resources/ResourceManager.h"

namespace dae::resources
{
	void ResourceManager::Init(const std::filesystem::path& dataPath)
	{
		m_dataPath = dataPath;

		if (!TTF_Init())
		{
			throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
		}
	}

	void ResourceManager::Destroy()
	{
		m_pLoadedFonts.clear();
		m_pLoadedTextures.clear();
	}

	std::shared_ptr<graphics::Texture2D> ResourceManager::LoadTexture(const std::string& file)
	{
		const auto full_path = m_dataPath / file;
		const auto filename = std::filesystem::path(full_path).filename().string();

		if (m_pLoadedTextures.find(filename) == m_pLoadedTextures.end())
		{
			m_pLoadedTextures.insert(std::pair(filename, std::make_shared<graphics::Texture2D>(full_path.string())));
		}

		return m_pLoadedTextures.at(filename);
	}

	std::shared_ptr<graphics::Font> ResourceManager::LoadFont(const std::string& file, uint8_t size)
	{
		const auto full_path = m_dataPath / file;
		const auto filename = std::filesystem::path(full_path).filename().string();
		const auto key = std::pair<std::string, uint8_t>(filename, size);

		if (m_pLoadedFonts.find(key) == m_pLoadedFonts.end())
		{
			m_pLoadedFonts.insert(std::pair(key, std::make_shared<graphics::Font>(full_path.string(), size)));
		}

		return m_pLoadedFonts.at(key);
	}

	void ResourceManager::UnloadUnusedResources()
	{
		std::erase_if(m_pLoadedTextures, [](const auto& item)
			{
			return item.second.use_count() == 1;
			});

		std::erase_if(m_pLoadedFonts, [](const auto& item)
			{
			return item.second.use_count() == 1;
			});
	}
}
