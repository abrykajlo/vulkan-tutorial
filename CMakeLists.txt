cmake_minimum_required(VERSION 3.14)
project(VulkanTutorial)

find_package(Vulkan REQUIRED)

include(FetchContent)

# GLFW
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
)
FetchContent_MakeAvailable(glfw)
set(GLFW_INCLUDE_DIRS "${GLFW_SOURCE_DIR}/include" PARENT_SCOPE)

# GLM
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
)
FetchContent_MakeAvailable(glm)

add_subdirectory(src)
