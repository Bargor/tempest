// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {

    namespace vulkan {

        struct queue_family_indices {
            std::optional<std::uint32_t> graphicsIndex;
            std::optional<std::uint32_t> computeIndex;
            std::optional<std::uint32_t> presentationIndex;
            std::optional<std::uint32_t> transferIndex;

            inline bool isValid() {
                return graphicsIndex && computeIndex && presentationIndex && transferIndex;
            }
        };

        queue_family_indices compute_queue_indices(const vk::PhysicalDevice& physicalDevice,
                                                   const vk::SurfaceKHR& windowSurface);

    } // namespace vulkan

} // namespace engine
} // namespace tst