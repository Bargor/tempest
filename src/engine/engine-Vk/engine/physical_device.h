// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

namespace tst {
namespace engine {

    namespace vulkan {

        class physical_device {
            friend class logical_device;
            friend class swap_chain;
            friend VkDevice create_logical_device(const physical_device& physicalDevice,
                                                  const std::vector<const char*>& validationLayers,
                                                  const std::vector<const char*>& extensions,
                                                  bool enableValidationLayers);

        public:
            struct queue_family_indices {
                std::optional<std::uint32_t> graphicsIndex;
                std::optional<std::uint32_t> computeIndex;
                std::optional<std::uint32_t> presentationIndex;
                std::optional<std::uint32_t> transferIndex;
            };

            physical_device(const VkPhysicalDevice& handle,
                            const VkSurfaceKHR& windowSurface,
                            const std::vector<const char*>& requiredExtenstions);

            const queue_family_indices& get_queue_family_indices() const;

        private:

            bool has_required_queues() const;

        private:

            VkPhysicalDevice m_deviceHandle;
            queue_family_indices m_indices;
        };

    } // namespace vulkan

} // namespace engine
} // namespace tst