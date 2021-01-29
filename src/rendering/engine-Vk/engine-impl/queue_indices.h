// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <optional>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {

    namespace vulkan {

        struct queue_family_indices {
            std::optional<std::uint32_t> graphicsIndex;
            std::optional<std::uint32_t> computeIndex;
            std::optional<std::uint32_t> presentationIndex;
            std::optional<std::uint32_t> transferIndex;

            inline bool isValid(bool presentationEnabled) {
                if (presentationEnabled)
                    return graphicsIndex && computeIndex && presentationIndex && transferIndex;
                else
                    return graphicsIndex && computeIndex && transferIndex;
            }
        };

        queue_family_indices compute_queue_indices(const vk::SurfaceKHR windowSurface,
                                                   const vk::PhysicalDevice physicalDevice);

    } // namespace vulkan

} // namespace engine
} // namespace tst