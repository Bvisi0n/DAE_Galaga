#include <algorithm>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui_plot.h>

#include "Singletons/Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "ThrashCache.h"

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	m_renderer = SDL_CreateRenderer(window, nullptr);

	if (m_renderer == nullptr)
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
		// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
		// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
		io.IniFilename = NULL;
	#endif

	ImGui_ImplSDL3_InitForSDLRenderer(window, m_renderer);
	ImGui_ImplSDLRenderer3_Init(m_renderer);
}

void dae::Renderer::Render() const
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ThrashCacheExercise1();
	ThrashCacheExercise2();

	ImGui::Render();

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }

void dae::Renderer::ThrashCacheExercise1() const
{
	static int sample_count{ 10 };
	ImGui::Begin("Exercise 1");

	ThrashCacheDrawInput(sample_count);

	constexpr size_t buffer_size{ 67108864 }; // Left here intentionally (Could go in header)
	static std::vector<float> int_results;

	if (ImGui::Button("Thrash the cache"))
	{
		std::vector<int> test_data(buffer_size);
		int_results = dae::ThrashCache::MeasureCache(test_data, sample_count, [](int& value) { value *= 2; });
	}

	if (!int_results.empty())
	{
        ThrashCacheDrawGraph("Integer", int_results, ImColor{ 255, 0, 0 });
	}

	ImGui::End();
}

void dae::Renderer::ThrashCacheExercise2() const
{
	static int sample_count{ 100 };
	ImGui::Begin("Exercise 2");

    ThrashCacheDrawInput(sample_count);

	constexpr size_t buffer_size{ 67108864 }; // Left here intentionally (Could go in header)
	static std::vector<float> testObject_results;

	if (ImGui::Button("Thrash the cache with GameObject3D"))
	{
		std::vector<dae::ThrashCache::TestObject> test_data(buffer_size);
		testObject_results = dae::ThrashCache::MeasureCache(test_data, sample_count, [](dae::ThrashCache::TestObject& obj) { obj.ID *= 2; });
	}

	if (!testObject_results.empty())
	{
		ThrashCacheDrawGraph("TestObject", testObject_results, ImColor{ 0, 255, 0 });
	}

	static std::vector<float> testObjectAlt_results;

	if (ImGui::Button("Thrash the cache with GameObject3DAlt"))
	{
		std::vector<dae::ThrashCache::TestObjectAlt> test_data(buffer_size);
		testObjectAlt_results = dae::ThrashCache::MeasureCache(test_data, sample_count, [](dae::ThrashCache::TestObjectAlt& obj) { obj.ID *= 2; });
	}

	if (!testObjectAlt_results.empty())
	{
        ThrashCacheDrawGraph("TestObjectAlt", testObjectAlt_results, ImColor{ 0, 0, 255 });
	}

	if (!testObject_results.empty() && !testObjectAlt_results.empty())
	{
		ImGui::SeparatorText("Combined");
		const float* data_list[] = { testObject_results.data(), testObjectAlt_results.data() };
		static ImU32 colors[] = { ImColor(0, 255, 0), ImColor(0, 0, 255) };
		float max_val = std::max(*std::max_element(testObject_results.begin(), testObject_results.end()), *std::max_element(testObjectAlt_results.begin(), testObjectAlt_results.end()));
		ThrashCacheDrawGraph("Combined", data_list, colors, max_val, static_cast<int>(testObject_results.size()));
	}

	ImGui::End();
}

void dae::Renderer::ThrashCacheDrawInput(int& sampleCount) const
{
	ImGui::InputInt("##valueInput", &sampleCount);
	sampleCount = std::clamp(sampleCount, 1, 1000);
	ImGui::SameLine();
	ImGui::Text("# samples");
}

void dae::Renderer::ThrashCacheDrawGraph(const std::string name, const std::vector<float>& data, const ImColor color) const
{
	ImGui::PlotConfig plot_config;
	plot_config.values.ys = data.data();
	plot_config.values.count = static_cast<int>(data.size());
    plot_config.values.color = color;
	plot_config.scale.min = 0;
	plot_config.scale.max = *std::max_element(data.begin(), data.end()) * 1.1f;
	plot_config.tooltip.show = false;
	plot_config.grid_x.show = false;
	plot_config.grid_y.show = false;
	plot_config.frame_size = ImVec2(200, 100);
	plot_config.line_thickness = 2.0f;
	ImGui::Plot(name.c_str(), plot_config);
}

void dae::Renderer::ThrashCacheDrawGraph(const std::string name, const float* data_list[], const ImU32 colors[], const float maxElement, const int valueCount) const
{
	ImGui::PlotConfig plot_config;
	plot_config.values.ys_list = data_list;
	plot_config.values.ys_count = 2;
	plot_config.values.count = valueCount;
	plot_config.values.colors = colors;
	plot_config.scale.min = 0;
	plot_config.scale.max = maxElement * 1.1f;
	plot_config.grid_x.show = false;
	plot_config.grid_y.show = false;
	plot_config.frame_size = ImVec2(200, 100);
	plot_config.line_thickness = 2.0f;
	ImGui::Plot(name.c_str(), plot_config);
}
