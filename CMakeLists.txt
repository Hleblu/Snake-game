cmake_minimum_required(VERSION 3.22)
project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW TRUE
)

if (WIN32)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
endif()

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

add_executable(Snake
    "src/main.cpp" 
    "src/core/game.cpp"
    "src/entities/apple.cpp"
    "src/core/collisionManager.cpp"
    "src/graphics/flash.cpp"
    "src/graphics/floatingText.cpp"
    "src/core/menu.cpp"
    "src/entities/obstacle.cpp"
    "src/graphics/particleManager.cpp"
    "src/utils/pathUtils.cpp"
    "src/graphics/renderResources.cpp"
    "src/core/serializable.cpp"
    "src/entities/snake.cpp"
    "src/core/soundManager.cpp"
    "src/core/difficultyManager.cpp"
    "src/graphics/checkeredBG.cpp"
)

if (WIN32)
    target_sources(Snake PRIVATE src/winRes/Resources.rc)
endif()

target_include_directories(Snake PRIVATE
    include
    assets/
)

set_target_properties(Snake PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)

target_link_libraries(Snake PRIVATE 
    SFML::Graphics 
    SFML::Window
    SFML::System
    SFML::Audio
)

if(WIN32 OR APPLE)
    target_link_libraries(Snake PRIVATE SFML::Main)
endif()