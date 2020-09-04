// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "texture.h"

#include "../resource_cache.h"
#include "buffer.h"
#include "image.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vk::Sampler create_default_sampler(vk::Device device) {
            vk::SamplerCreateInfo samplerInfo(vk::SamplerCreateFlags(),
                                              vk::Filter::eLinear,
                                              vk::Filter::eLinear,
                                              vk::SamplerMipmapMode::eLinear,
                                              vk::SamplerAddressMode::eRepeat,
                                              vk::SamplerAddressMode::eRepeat,
                                              vk::SamplerAddressMode::eRepeat,
                                              0.0f,
                                              true,
                                              16,
                                              false,
                                              vk::CompareOp::eAlways,
                                              0.0f,
                                              0.0f,
                                              vk::BorderColor::eIntOpaqueBlack,
                                              false);

            return device.createSampler(samplerInfo);
        }

        texture::texture(const buffer_construction_info& info,
                         const resource_cache& resourceCache,
                         vk::BufferUsageFlags flags,
                         vk::MemoryPropertyFlags memoryFlags,
                         const application::image_data& imageData,
                         const std::uint32_t& resourceIndex,
                         vk::Sampler sampler)
            : m_logicalDevice(info.logicalDevice)
            , m_resourceCache(resourceCache)
            , m_sampler(sampler ? sampler : create_default_sampler(m_logicalDevice))
            , m_resourceIndex(resourceIndex) {
            buffer stagingBuffer(info, imageData.memorySize, flags, memoryFlags);
            stagingBuffer.copy_data(imageData.data.get(), imageData.memorySize);

            std::tie(m_textureImage, m_textureMemory) =
                create_image(m_logicalDevice,
                             {imageData.imageSize.width, imageData.imageSize.height},
                             vk::Format::eR8G8B8A8Srgb,
                             vk::ImageTiling::eOptimal,
                             vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
                             info.memoryProperties,
                             vk::MemoryPropertyFlagBits::eDeviceLocal);

            transition_image_layout(m_logicalDevice,
                                    info.queueHandle,
                                    info.cmdPool,
                                    m_textureImage,
                                    vk::Format::eR8G8B8A8Srgb,
                                    vk::ImageLayout::eUndefined,
                                    vk::ImageLayout::eTransferDstOptimal);
            copy_buffer_to_image(
                m_logicalDevice, info.cmdPool, info.queueHandle, stagingBuffer, m_textureImage, imageData.imageSize);
            transition_image_layout(m_logicalDevice,
                                    info.queueHandle,
                                    info.cmdPool,
                                    m_textureImage,
                                    vk::Format::eR8G8B8A8Srgb,
                                    vk::ImageLayout::eTransferDstOptimal,
                                    vk::ImageLayout::eShaderReadOnlyOptimal);

            m_textureView = create_image_view(
                m_logicalDevice, m_textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);
        }

        texture::~texture() {
            m_logicalDevice.destroyImageView(m_textureView);
            m_logicalDevice.destroyImage(m_textureImage);
            m_logicalDevice.freeMemory(m_textureMemory);
            m_logicalDevice.destroySampler(m_sampler);
        }

        void texture::bind_texture(const std::string& shaderName,
                                   base::resource_bind_point bindPoint,
                                   std::uint32_t binding) {
            m_descriptorSets = m_resourceCache.find_descriptor_sets(shaderName, bindPoint);
            assert(m_descriptorSets);

            for (const auto set : *m_descriptorSets) {
                vk::DescriptorImageInfo imageInfo(m_sampler, m_textureView, vk::ImageLayout::eShaderReadOnlyOptimal);

                vk::WriteDescriptorSet descriptorWrite(
                    set, binding, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo, nullptr, nullptr);

                m_logicalDevice.updateDescriptorSets({descriptorWrite}, {});
            }
        }

        texture::texture(texture&& other) noexcept
            : m_logicalDevice(other.m_logicalDevice)
            , m_resourceCache(other.m_resourceCache)
            , m_textureImage(other.m_textureImage)
            , m_textureMemory(other.m_textureMemory)
            , m_textureView(other.m_textureView)
            , m_sampler(other.m_sampler)
            , m_resourceIndex(other.m_resourceIndex)
            , m_descriptorSets(other.m_descriptorSets) {
            other.m_textureImage = nullptr;
            other.m_textureMemory = nullptr;
            other.m_textureView = nullptr;
            other.m_sampler = nullptr;
            other.m_descriptorSets = nullptr;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
