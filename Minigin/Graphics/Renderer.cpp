#include <algorithm>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Graphics/Texture2D.h"
#include "Minigin/Scene/SceneManager.h"

namespace dae::graphics
{
	void Renderer::Init(SDL_Window* pWindow)
	{
		m_pWindow = pWindow;
		SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
		m_pRenderer = SDL_CreateRenderer(pWindow, nullptr);

		if (m_pRenderer == nullptr)
		{
			std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
			throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Enable Gamepad Controls

		#if __EMSCRIPTEN__
		// For an Emscripten build we are disabling file-system access.
		// Use LoadIniSettingsFromMemory() to load settings from your own storage.
		io.IniFilename = NULL;
		#endif

		ImGui_ImplSDL3_InitForSDLRenderer(pWindow, m_pRenderer);
		ImGui_ImplSDLRenderer3_Init(m_pRenderer);
	}

	void Renderer::Render() const
	{
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		const auto& color = GetBackgroundColor();
		SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);

		SDL_RenderClear(m_pRenderer);

		scene::SceneManager::GetInstance().Render();

		ImGui::Render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_pRenderer);
		SDL_RenderPresent(m_pRenderer);
	}

	void Renderer::Destroy()
	{
		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();

		if (m_pRenderer != nullptr)
		{
			SDL_DestroyRenderer(m_pRenderer);
			m_pRenderer = nullptr;
		}
	}

	void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
	{
		SDL_FRect dst{};
		dst.x = x;
		dst.y = y;
		SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
		SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
	}

	void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
	{
		SDL_FRect dst{};
		dst.x = x;
		dst.y = y;
		dst.w = width;
		dst.h = height;
		SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
	}

	SDL_Renderer* Renderer::GetSDLRenderer() const
	{
		return m_pRenderer;
	}
}
