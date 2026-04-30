set(ENGINE_SOURCES
	"Audio/LoggingSoundSystem.cpp"
	"Core/AppStateManager.cpp"
	"Core/ColliderComponent.cpp"
	"Core/CollisionSystem.cpp"
	"Core/GameObject.cpp"
	"Core/Minigin.cpp"
	"Core/MoveComponent.cpp"
	"Core/ServiceLocator.cpp"
	"Core/Transform.cpp"
	"Events/ObservableComponent.cpp"
	"Graphics/Font.cpp"
	"Graphics/Renderer.cpp"
	"Graphics/TextComponent.cpp"
	"Graphics/TextureComponent.cpp"
	"Graphics/Texture2D.cpp"
	"Input/Gamepad.cpp"
	"Input/InputManager.cpp"
	"Input/Keyboard.cpp"
	"Resources/ResourceManager.cpp"
	"Scene/Scene.cpp"
	"Scene/SceneManager.cpp"
)

if(NOT EMSCRIPTEN)
	list(APPEND ENGINE_SOURCES
		"Audio/SDLSoundSystem.cpp"
	)
endif()

add_library(Minigin STATIC ${ENGINE_SOURCES})

target_compile_features(Minigin PUBLIC cxx_std_20)
target_include_directories(Minigin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_include_directories(Minigin SYSTEM PUBLIC
	${glm_SOURCE_DIR}
	${sdl3_SOURCE_DIR}/include
	${sdl3_image_SOURCE_DIR}/include
	${sdl3_ttf_SOURCE_DIR}/include
	${imgui_SOURCE_DIR}
	${VLD_INCLUDE_DIR}
)

target_link_libraries(Minigin PUBLIC
	SDL3::SDL3
	SDL3_ttf::SDL3_ttf
	imgui
	glm::glm
)

if(NOT EMSCRIPTEN)
	target_link_libraries(Minigin PRIVATE SDL3_mixer::SDL3_mixer)
endif()

if(WIN32 AND VLD_FOUND)
	target_link_libraries(Minigin PUBLIC ${VLD_LIBRARIES})
endif()
