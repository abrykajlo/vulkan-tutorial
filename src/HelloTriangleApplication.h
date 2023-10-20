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

private:
  SDL_Window *m_window;
  VkInstance m_instance;
  VkDebugUtilsMessengerEXT m_debugMessenger;
};
