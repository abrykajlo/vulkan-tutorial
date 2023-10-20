#include "HelloTriangleApplication.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char *> g_validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

bool checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : g_validationLayers) {
    auto it = std::find_if(availableLayers.begin(), availableLayers.end(),
                           [layerName](const auto &layer) {
                             return strcmp(layerName, layer.layerName) == 0;
                           });

    if (it == availableLayers.end()) {
      return false;
    }
  }
  return true;
}

std::vector<const char *> getRequiredExtensions() {
  uint32_t extensionCount = 0;
  SDL_Vulkan_GetInstanceExtensions(&extensionCount, nullptr);
  std::vector<const char *> extensions(extensionCount);
  SDL_Vulkan_GetInstanceExtensions(&extensionCount, extensions.data());

  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {
  if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
  }

  return VK_FALSE;
}

void populateDebugsUtilsMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;
}

VkResult
createDebugUtilsMessenger(VkInstance instance,
                          const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                          const VkAllocationCallbacks *pAllocator,
                          VkDebugUtilsMessengerEXT *pMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void destroyDebugUtilsMessenger(VkInstance instance,
                                VkDebugUtilsMessengerEXT messenger,
                                const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, messenger, pAllocator);
  }
}
} // namespace

void HelloTriangleApplication::run() {
  initWindow();
  initVulkan();
  mainLoop();
  cleanup();
}

void HelloTriangleApplication::initWindow() {
  SDL_Init(SDL_INIT_VIDEO);
  m_window = SDL_CreateWindow("Vulkan", WIDTH, HEIGHT,
                              SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
}

void HelloTriangleApplication::initVulkan() {
  createInstance();
  setupDebugMessenger();
}

void HelloTriangleApplication::createInstance() {
  if (enableValidationLayers && !checkValidationLayerSupport()) {
    throw std::runtime_error("validation layers requested, but not available!");
  }

  // get extensions from vulkan
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         extensions.data());

  std::cout << "available extensions:\n";
  for (const auto &extension : extensions) {
    std::cout << '\t' << extension.extensionName << '\n';
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(g_validationLayers.size());
    createInfo.ppEnabledLayerNames = g_validationLayers.data();
    populateDebugsUtilsMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = &debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  bool allExtensionsAvailable = true;
  std::cout << "required extensions:\n";
  std::vector<const char *> requiredExtensionNames = getRequiredExtensions();
  for (auto requiredExtensionName : requiredExtensionNames) {
    std::cout << '\t' << requiredExtensionName << '\n';

    auto it = std::find_if(extensions.begin(), extensions.end(),
                           [requiredExtensionName](const auto &extension) {
                             return strcmp(requiredExtensionName,
                                           extension.extensionName) == 0;
                           });
  }
  if (allExtensionsAvailable) {
    std::cout << "all extensions available\n";
  } else {
    std::cout << "not all extensions available\n";
  }

  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(requiredExtensionNames.size());
  createInfo.ppEnabledExtensionNames = requiredExtensionNames.data();
  createInfo.enabledLayerCount = 0;

  if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}

void HelloTriangleApplication::mainLoop() {
  bool done = false;
  while (!done) {
    SDL_Event event;
    SDL_PumpEvents();
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }
  }
}

void HelloTriangleApplication::cleanup() {
  if (enableValidationLayers) {
    destroyDebugUtilsMessenger(m_instance, m_debugMessenger, nullptr);
  }

  vkDestroyInstance(m_instance, nullptr);

  SDL_DestroyWindow(m_window);

  SDL_Quit();
}

void HelloTriangleApplication::setupDebugMessenger() {
  if (!enableValidationLayers)
    return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  populateDebugsUtilsMessengerCreateInfo(createInfo);

  if (createDebugUtilsMessenger(m_instance, &createInfo, nullptr,
                                &m_debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger");
  }
}
