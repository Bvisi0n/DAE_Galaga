#ifndef DAE_FONT_H
#define DAE_FONT_H

#include <memory>
#include <string>

struct TTF_Font;

namespace dae::graphics
{
	class Font final
	{
	public:
		explicit Font( const std::string& fullPath, float size );
		~Font() = default;

		Font( const Font& ) = delete;
		Font( Font&& ) = delete;
		Font& operator= ( const Font& ) = delete;
		Font& operator= ( const Font&& ) = delete;

		[[nodiscard]] TTF_Font* GetFont() const noexcept;

	private:
		std::unique_ptr<TTF_Font, void( * )( TTF_Font* )> m_font{ nullptr, nullptr };
	};
}
#endif
