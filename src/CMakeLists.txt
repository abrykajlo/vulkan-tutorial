add_executable(VulkanTutorial main.cpp)

target_include_directories(VulkanTutorial PRIVATE ..
  ${GLFW_INCLUDE_DIRS}
  ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanTutorial glm ${Vulkan_LIBRARIES} glfw)
