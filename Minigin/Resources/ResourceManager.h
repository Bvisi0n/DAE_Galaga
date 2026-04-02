#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "Minigin/Utilities/Singleton.h"

namespace dae
{
	class Font;
	class Texture2D;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::filesystem::path& data);
		void Destroy();
		
		[[nodiscard]] std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		[[nodiscard]] std::shared_ptr<Font>		 LoadFont(const std::string& file, uint8_t size);

	private:
		friend class Singleton<ResourceManager>;
		
		std::filesystem::path m_dataPath;
		std::map<std::string, std::shared_ptr<Texture2D>> m_pLoadedTextures;
		std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_pLoadedFonts;
		
		ResourceManager() = default;
		virtual ~ResourceManager() = default;
		void UnloadUnusedResources();
	};
}
#endif
