if(EMSCRIPTEN)
  set(TARGET_NAME minigin_web)
  set(TARGET_PARAM )
else()
  set(TARGET_NAME minigin)
  set(TARGET_PARAM WIN32)
endif()

# Flag to control inclusion of Gravity Bender prototype
option(BUILD_GRAVITY_BENDER "Include Gravity Bender prototype" ON)

# Define Common and Galaga Sources (Always Built)
set(GAME_SOURCES
  "Core/Main.cpp"

  "Common/FPSComponent.cpp"
  
  "Galaga/GalagaState.cpp"
  "Galaga/HealthComponent.cpp"
  "Galaga/RotatorComponent.cpp"
  "Galaga/ScoreComponent.cpp"
  "Galaga/UIValueObserver.cpp"
)

# Add all Gravity Bender files here
if(BUILD_GRAVITY_BENDER)
    message(STATUS "Game Module: Gravity Bender ENABLED")
    list(APPEND GAME_SOURCES
        "GravityBender/SpawnerPortalComponent.cpp"
        "GravityBender/PlayerComponent.cpp"
    )
else()
    message(STATUS "Game Module: Gravity Bender DISABLED (Strict Galaga Build)")
endif()

# Generate Executable
add_executable(${TARGET_NAME} ${TARGET_PARAM} ${GAME_SOURCES} )

# Inject the preprocessor macro
if(BUILD_GRAVITY_BENDER)
    target_compile_definitions(${TARGET_NAME} PRIVATE ENABLE_GRAVITY_BENDER)
endif()

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

# Link the engine securely to the game
target_link_libraries(${TARGET_NAME} PRIVATE Minigin)

if(WIN32)
    target_link_libraries(${TARGET_NAME} PRIVATE xinput.lib)
endif()

# Compiler Configurations
if(MSVC)
  string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
  string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")
  string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  target_compile_options(${TARGET_NAME} PRIVATE
   /W4
   /WX
   /permissive-
   /EHs-c-
  )
  target_compile_definitions(${TARGET_NAME} PRIVATE
    _HAS_EXCEPTIONS=0
  )
else()
  target_compile_options(${TARGET_NAME} PRIVATE
   -Wall
   -Wextra
   -Werror
   -Winconsistent-missing-override
   -fno-exceptions
   -fno-unwind-tables
  )
endif()

# Static Analysis Integration
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" ON)

if(ENABLE_CLANG_TIDY AND NOT EMSCRIPTEN)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy" "clang-tidy.exe")
    if(CLANG_TIDY_EXE)
        message(STATUS "Static Analysis: clang-tidy found.")
        set_target_properties(${TARGET_NAME} PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
        )
    endif()
endif()

# Platform-Specific Post-Build Operations
if(EMSCRIPTEN)
  set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g")
  set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\"")

  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/EmscriptenToolchain.cmake)

  set_target_properties(${TARGET_NAME} PROPERTIES
      COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
      LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
  )

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/web" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
  )

  set(CMAKE_EXECUTABLE_SUFFIX ".html")

elseif(WIN32)
  set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
  set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

  set(SDL_RUNTIME_DLLS SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_mixer::SDL3_mixer)

  foreach(dllTarget IN LISTS SDL_RUNTIME_DLLS)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "$<TARGET_FILE:${dllTarget}>"
          "$<TARGET_FILE_DIR:${TARGET_NAME}>"
      VERBATIM
    )
  endforeach()

  if(Steamworks_FOUND)
    target_link_libraries("${TARGET_NAME}" PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${TARGET_NAME} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy "${Steamworks_RUNTIME_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>")
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${TARGET_NAME}>/steam_appid.txt"
    )
  endif()

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )

elseif(APPLE)
  set_target_properties(${TARGET_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    INSTALL_RPATH "@executable_path/../Frameworks"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks/"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared>" "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks/"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_mixer::SDL3_mixer-shared>" "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks/"
    COMMAND install_name_tool -change "@rpath/libSDL3_ttf.0.dylib" "@executable_path/../Frameworks/libSDL3_ttf.0.2.2.dylib" "$<TARGET_FILE:${TARGET_NAME}>"
    COMMAND install_name_tool -change "@rpath/libSDL3.0.dylib" "@executable_path/../Frameworks/libSDL3.0.dylib" "$<TARGET_FILE:${TARGET_NAME}>"
  )

  if(Steamworks_FOUND)
    target_link_libraries("${TARGET_NAME}" PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${TARGET_NAME} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks/"
    )
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${TARGET_NAME}>/steam_appid.txt"
    )
  endif()

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )

else()
  if(Steamworks_FOUND)
    target_link_libraries("${TARGET_NAME}" PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${TARGET_NAME} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${TARGET_NAME}>/steam_appid.txt"
    )
  endif()

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )
endif()
