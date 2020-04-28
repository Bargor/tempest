// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "texture.h"

#include "..\resource_cache.h"
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

        texture::texture(vk::Device logicalDevice,
                         vk::Queue queueHandle,
                         vk::CommandPool cmdPool,
                         vk::DescriptorPool descPool,
                         const resource_cache& resourceCache,
                         vk::BufferUsageFlags flags,
                         const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                         vk::MemoryPropertyFlags memoryFlags,
                         const application::image_data& imageData,
                         vk::Sampler sampler)
            : m_logicalDevice(logicalDevice)
            , m_descPool(descPool)
            , m_resourceCache(resourceCache)
            , m_sampler(sampler ? sampler : create_default_sampler(m_logicalDevice)) {
            buffer stagingBuffer(
                logicalDevice, queueHandle, cmdPool, imageData.memorySize, flags, memoryProperties, memoryFlags);

            std::tie(m_textureImage, m_textureMemory) =
                create_image(logicalDevice,
                             {imageData.imageSize.width, imageData.imageSize.height},
                             vk::Format::eR8G8B8A8Srgb,
                             vk::ImageTiling::eOptimal,
                             vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
                             memoryProperties,
                             vk::MemoryPropertyFlagBits::eDeviceLocal);

            transition_image_layout(logicalDevice,
                                    queueHandle,
                                    cmdPool,
                                    m_textureImage,
                                    vk::Format::eR8G8B8Srgb,
                                    vk::ImageLayout::eUndefined,
                                    vk::ImageLayout::eTransferDstOptimal);
            copy_buffer_to_image(logicalDevice, cmdPool, queueHandle, stagingBuffer, m_textureImage, imageData.imageSize);
            transition_image_layout(logicalDevice,
                                    queueHandle,
                                    cmdPool,
                                    m_textureImage,
                                    vk::Format::eR8G8B8Srgb,
                                    vk::ImageLayout::eTransferDstOptimal,
                                    vk::ImageLayout::eShaderReadOnlyOptimal);

            m_textureView = create_image_view(
                logicalDevice, m_textureImage, vk::Format::eR8G8B8Srgb, vk::ImageAspectFlagBits::eColor);
        }

        texture::~texture() {
            m_logicalDevice.destroyImageView(m_textureView);
            m_logicalDevice.destroyImage(m_textureImage);
            m_logicalDevice.freeMemory(m_textureMemory);
            m_logicalDevice.destroySampler(m_sampler);
        }

        void texture::bind_texture(const std::string& shaderName, shader_type type, std::uint32_t binding) {
            auto descLayout = [this, &shaderName, type, binding]() {
                const auto shaders = m_resourceCache.find_shaders(shaderName);
                for (const auto& shader : *shaders) {
                    if (shader.get_stage() == type) {
                        return m_resourceCache.find_descriptor_layout(shader.get_layouts()[0]);
                    } else
                        continue;
                }
                throw;
            }();

            const vk::DescriptorSetAllocateInfo allocInfo(m_descPool, 1, descLayout);

            m_descriptorSet = m_logicalDevice.allocateDescriptorSets(allocInfo)[0];

            vk::DescriptorImageInfo imageInfo(m_sampler, m_textureView, vk::ImageLayout::eShaderReadOnlyOptimal);

            vk::WriteDescriptorSet descriptorWrite(
                m_descriptorSet, binding, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo, nullptr, nullptr);

            m_logicalDevice.updateDescriptorSets({descriptorWrite}, {});
        }

        texture::texture(texture&& other) noexcept
            : m_logicalDevice(other.m_logicalDevice)
            , m_descPool(other.m_descPool)
            , m_resourceCache(other.m_resourceCache)
            , m_textureImage(other.m_textureImage)
            , m_textureMemory(other.m_textureMemory)
            , m_textureView(other.m_textureView)
            , m_sampler(other.m_sampler)
            , m_descriptorSet (other.m_descriptorSet){
            other.m_textureImage = nullptr;
            other.m_textureMemory = nullptr;
            other.m_textureView = nullptr;
            other.m_sampler = nullptr;
            other.m_descriptorSet = nullptr;
        }

        vk::DescriptorSet texture::get_descriptor_set() const noexcept {
            return m_descriptorSet;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
