cmake_minimum_required(VERSION 3.7)
project(sdl2_libretro_test)

set(CMAKE_CXX_STANDARD 14)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

# Make Libretro library
add_library(${PROJECT_NAME} SHARED libretro.hpp libretro.cpp game.cpp game.hpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

# Make exe
add_executable(${PROJECT_NAME}_exe main.cpp game.cpp game.hpp)
target_link_libraries(${PROJECT_NAME}_exe ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})