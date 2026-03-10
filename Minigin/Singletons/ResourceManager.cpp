#include <stdexcept>

#include <SDL3_ttf/SDL_ttf.h>

#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#include "Font.h"
#include "Texture2D.h"

namespace fs = std::filesystem;

void dae::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_dataPath = dataPath;

	if (!TTF_Init())
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	if(m_pLoadedTextures.find(filename) == m_pLoadedTextures.end())
		m_pLoadedTextures.insert(std::pair(filename,std::make_shared<Texture2D>(fullPath.string())));
	return m_pLoadedTextures.at(filename);
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	const auto key = std::pair<std::string, uint8_t>(filename, size);
	if(m_pLoadedFonts.find(key) == m_pLoadedFonts.end())
		m_pLoadedFonts.insert(std::pair(key,std::make_shared<Font>(fullPath.string(), size)));
	return m_pLoadedFonts.at(key);
}

void dae::ResourceManager::UnloadUnusedResources()
{
	for (auto it = m_pLoadedTextures.begin(); it != m_pLoadedTextures.end();)
	{
		if (it->second.use_count() == 1)
			it = m_pLoadedTextures.erase(it);
		else
			++it;
	}

	for (auto it = m_pLoadedFonts.begin(); it != m_pLoadedFonts.end();)
	{
		if (it->second.use_count() == 1)
			it = m_pLoadedFonts.erase(it);
		else
			++it;
	}
}
