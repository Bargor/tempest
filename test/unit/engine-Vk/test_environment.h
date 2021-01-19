// This file is part of Tempest-engine project
// Author: Karol Kontny

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

inline GLFWwindow* g_glfwWindow;
inline vk::SurfaceKHR g_surface;

vk::PhysicalDeviceFeatures get_required_features();