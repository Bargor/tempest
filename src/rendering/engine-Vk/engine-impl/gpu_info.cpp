// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "gpu_info.h"

namespace tst {
namespace engine {
    namespace vulkan {
        gpu_info::gpu_info(const vk::PhysicalDevice& physicalDevice) {
            auto deviceProperties = physicalDevice.getProperties();
            super::versionMajor = VK_VERSION_MAJOR(deviceProperties.apiVersion);
            super::versionMinor = VK_VERSION_MINOR(deviceProperties.apiVersion);
            super::versionPatch = VK_VERSION_PATCH(deviceProperties.apiVersion);

            switch (deviceProperties.deviceType) {
            case vk::PhysicalDeviceType::eDiscreteGpu: 
                super::deviceType = device_type::discrete;
                break;
            case vk::PhysicalDeviceType::eIntegratedGpu:
                super::deviceType = device_type::integrated;
                break;
            case vk::PhysicalDeviceType::eCpu: 
                super::deviceType = device_type::cpu;
                break;
            default: super::deviceType = device_type::other;
            }
            switch (deviceProperties.vendorID) {
            case 0x10DE:
                super::deviceVendor = device_vendor::nvidia;
                break;
            case 0x1002: 
                super::deviceVendor = device_vendor::amd;
                break; 
            case 0x8086:
                super::deviceVendor = device_vendor::intel;
                break;
            default:
                super::deviceVendor = device_vendor::other;
            }
            super::deviceName = deviceProperties.deviceName;
            deviceProperties.limits;

            maxIndexValue = deviceProperties.limits.maxDrawIndexedIndexValue;
            maxFramebufferWidth = deviceProperties.limits.maxFramebufferWidth;
            maxFramebufferHeight = deviceProperties.limits.maxFramebufferHeight;
            maxColorAttachments = deviceProperties.limits.maxColorAttachments;
            maxImageDimension2D = deviceProperties.limits.maxImageDimension2D;
            maxVertexInputAttributes = deviceProperties.limits.maxVertexInputAttributes;
            maxVertexInputBindings = deviceProperties.limits.maxVertexInputBindings;

            maxPushConstantSize = deviceProperties.limits.maxPushConstantsSize;
            maxMemoryAllocationCount = deviceProperties.limits.maxMemoryAllocationCount;
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst