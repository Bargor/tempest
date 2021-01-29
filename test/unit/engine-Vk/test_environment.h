// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

inline GLFWwindow* g_glfwWindow;

vk::PhysicalDeviceFeatures get_required_features();