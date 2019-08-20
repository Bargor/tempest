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

        class physical_device {
        public:
            physical_device(vk::PhysicalDevice device, ptr<gpu_info> info, queue_family_indices& indices);

            vk::PhysicalDevice& get_device_handle() noexcept;
            std::uint32_t get_graphics_index() const noexcept;
            std::uint32_t get_transfer_index() const noexcept;
            std::uint32_t get_compute_index() const noexcept;
            std::uint32_t get_presentation_index() const noexcept;
            gpu_info& get_GPU_info() const noexcept;

            vk::Device create_logical_device(const std::vector<const char*>& validationLayers,
                                             const std::vector<const char*>& extensions);

        private:
            vk::PhysicalDevice m_physicalDevice;
            ptr<gpu_info> m_gpuInfo;
            queue_family_indices m_queueIndices;
        };

        TST_INLINE vk::PhysicalDevice& physical_device::get_device_handle() noexcept {
            return m_physicalDevice;
        }

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

    } // namespace vulkan
} // namespace engine
} // namespace tst
