#pragma once

#include <vulkan/vulkan.h>

class SDL_Window;

class HelloTriangleApplication {
public:
  void run();

private:
  void initWindow();
  void initVulkan();
  void createInstance();
  void mainLoop();
  void cleanup();
  void setupDebugMessenger();
  void pickPhysicalDevice();
  void createLogicalDevice();

private:
  SDL_Window *m_window = nullptr;
  VkInstance m_instance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
  VkDevice m_device = VK_NULL_HANDLE;
};
