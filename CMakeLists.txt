cmake_minimum_required(VERSION 3.14)
project(VulkanTutorial)

find_package(Vulkan REQUIRED)

include(FetchContent)

set(SDL_STATIC ON)
# SDL2
FetchContent_Declare(sdl3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG main
)
FetchContent_MakeAvailable(sdl3)
set(sdl3_INCLUDE_DIRS "${sdl3_SOURCE_DIR}/include")

# GLM
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
)
FetchContent_MakeAvailable(glm)

add_subdirectory(src)
