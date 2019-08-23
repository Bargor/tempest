// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once
#include "api.h"
#include "queue_indices.h"

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class gpu_info;

        struct surface_support_info {
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> formats;
            std::vector<vk::PresentModeKHR> presentModes;
        };

        class physical_device {
        public:
            physical_device(vk::PhysicalDevice device, ptr<gpu_info> info, queue_family_indices& indices);

            std::uint32_t get_graphics_index() const noexcept;
            std::uint32_t get_transfer_index() const noexcept;
            std::uint32_t get_compute_index() const noexcept;
            std::uint32_t get_presentation_index() const noexcept;
            gpu_info& get_GPU_info() const noexcept;

            vk::PhysicalDeviceMemoryProperties get_memory_properties() const noexcept;
            surface_support_info get_surface_support_info(vk::SurfaceKHR windowSurface) const;
            vk::Device create_logical_device(const std::vector<const char*>& validationLayers,
                                             const std::vector<const char*>& extensions);

        private:
            vk::PhysicalDevice m_physicalDevice;
            ptr<gpu_info> m_gpuInfo;
            queue_family_indices m_queueIndices;
        };

        TST_INLINE std::uint32_t physical_device::get_graphics_index() const noexcept {
            return m_queueIndices.graphicsIndex.value();
        }

        TST_INLINE std::uint32_t physical_device::get_transfer_index() const noexcept {
            return m_queueIndices.transferIndex.value();
        }

        TST_INLINE std::uint32_t physical_device::get_compute_index() const noexcept {
            return m_queueIndices.computeIndex.value();
        }

        TST_INLINE std::uint32_t physical_device::get_presentation_index() const noexcept {
            return m_queueIndices.presentationIndex.value();
        }

        TST_INLINE gpu_info& physical_device::get_GPU_info() const noexcept {
            return *m_gpuInfo;
        }

        TST_INLINE vk::PhysicalDeviceMemoryProperties physical_device::get_memory_properties() const noexcept {
            return m_physicalDevice.getMemoryProperties();
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst