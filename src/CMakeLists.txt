add_executable(VulkanTutorial main.cpp HelloTriangleApplication.cpp HelloTriangleApplication.h)
set_target_properties(VulkanTutorial PROPERTIES CXX_STANDARD 17)
target_include_directories(VulkanTutorial PRIVATE ..
  ${sdl3_INCLUDE_DIRS}
  ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanTutorial glm ${Vulkan_LIBRARIES} SDL3-static)
