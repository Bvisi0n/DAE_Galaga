#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "Minigin/Utilities/Singleton.h"

namespace dae::graphics
{
	class Font;
	class Texture2D;
}

namespace dae::resources
{
	class ResourceManager final : public utils::Singleton<ResourceManager>
	{
	public:
		void Init( const std::filesystem::path& dataPath );
		void Destroy();

		[[nodiscard]] std::shared_ptr<graphics::Texture2D> LoadTexture( const std::string& file );
		[[nodiscard]] std::shared_ptr<graphics::Font> LoadFont( const std::string& file, uint8_t size );

	private:
		friend class utils::Singleton<ResourceManager>;

		std::filesystem::path m_dataPath;
		std::map<std::string, std::shared_ptr<graphics::Texture2D>> m_loadedTextures;
		std::map<std::pair<std::string, uint8_t>, std::shared_ptr<graphics::Font>> m_loadedFonts;

		ResourceManager() = default;
		virtual ~ResourceManager() = default;
		void UnloadUnusedResources();
	};
}
#endif
