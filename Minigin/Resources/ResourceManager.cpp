#include <cassert>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <SDL3_ttf/SDL_ttf.h>

#include "Minigin/Graphics/Font.h"
#include "Minigin/Graphics/Texture2D.h"
#include "Minigin/Resources/ResourceManager.h"

namespace dae::resources
{
	void ResourceManager::Init( const std::filesystem::path& dataPath )
	{
		m_dataPath = dataPath;

		const bool ttfInitialized = TTF_Init();

		if ( ttfInitialized == false )
		{
			assert( ttfInitialized == true && "Failed to load support for fonts" );
			return;
		}
	}

	void ResourceManager::Destroy()
	{
		m_loadedFonts.clear();
		m_loadedTextures.clear();
	}

	std::shared_ptr<graphics::Texture2D> ResourceManager::LoadTexture( const std::string& file )
	{
		const auto fullPath = m_dataPath / file;
		const auto filename = std::filesystem::path( fullPath ).filename().string();

		if ( m_loadedTextures.find( filename ) == m_loadedTextures.end() )
		{
			m_loadedTextures.insert( std::pair( filename, std::make_shared<graphics::Texture2D>( fullPath.string() ) ) );
		}

		return m_loadedTextures.at( filename );
	}

	std::shared_ptr<graphics::Font> ResourceManager::LoadFont( const std::string& file, uint8_t size )
	{
		const std::filesystem::path fullPath = m_dataPath / file;

		const std::string fileName = fullPath.filename().string();
		const auto fontKey = std::pair<std::string, uint8_t>( fileName, size );

		auto it = m_loadedFonts.find( fontKey );

		if ( it == m_loadedFonts.end() )
		{
			auto font = std::make_shared<graphics::Font>( fullPath.string(), size );

			auto emplaceResult = m_loadedFonts.emplace( fontKey, std::move( font ) );
			it = emplaceResult.first;
		}

		return it->second;
	}

	void ResourceManager::UnloadUnusedResources()
	{
		std::erase_if( m_loadedTextures, [] ( const auto& item )
			 {
				  return item.second.use_count() == 1;
			 } );

		std::erase_if( m_loadedFonts, [] ( const auto& item )
			  {
					return item.second.use_count() == 1;
			  } );
	}
}
