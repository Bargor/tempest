// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <filesystem>

inline GLFWwindow* g_glfwWindow;
inline std::filesystem::path g_executionDirectory;

vk::PhysicalDeviceFeatures get_required_features();