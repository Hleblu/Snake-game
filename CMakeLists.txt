cmake_minimum_required(VERSION 3.16)
project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (WIN32)
    set(SFML_STATIC_LIBRARIES ON)
endif()

if (WIN32 OR APPLE)
    find_package(SFML 3 COMPONENTS Graphics Window System Audio Main REQUIRED)
else()
    find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)
endif()

add_executable(Snake
    src/main.cpp 
    src/game.cpp
    src/apple.cpp
    src/collisionManager.cpp
    src/flash.cpp
    src/floatingText.cpp
    src/menu.cpp
    src/obstacle.cpp
    src/particleManager.cpp
    src/pathUtils.cpp
    src/renderResources.cpp
    src/serializable.cpp
    src/snake.cpp
    src/soundManager.cpp
)

if (WIN32)
    target_sources(Snake PRIVATE src/Resources.rc)
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