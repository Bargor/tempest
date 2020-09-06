// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "device.h"
#include "physical_device.h"
#include "resource_cache.h"
#include "resources/pipeline.h"
#include "resources/vertex_buffer.h"
#include "shader_compiler.h"
#include "swap_chain.h"

#include <application/data_loader.h>
#include <engine-base/pipeline_parser.h>
#include <engine-base/resource_bind_point.h>
#include <engine-base/technique_parser.h>
#include <fmt/format.h>

namespace tst {
namespace engine {
    namespace vulkan {

        resource_factory::resource_factory(const device& device, const application::data_loader& dataLoader)
            : m_device(device)
            , m_dataLoader(dataLoader)
            , m_shaderCompiler(std::make_unique<shader_compiler>(m_dataLoader, m_device.m_logicalDevice))
            , m_transferCommandPool(m_device.m_logicalDevice.createCommandPool(vk::CommandPoolCreateInfo(
                  vk::CommandPoolCreateFlags(), m_device.m_physicalDevice->get_graphics_index())))
            , m_transferQueue(m_device.m_graphicsQueueHandle) {
        }

        resource_factory::~resource_factory() {
            m_device.m_logicalDevice.destroyCommandPool(m_transferCommandPool);
        }

        resource_factory::resource_factory(resource_factory&& factory) noexcept
            : m_device(factory.m_device)
            , m_dataLoader(factory.m_dataLoader)
            , m_shaderCompiler(std::move(factory.m_shaderCompiler))
            , m_transferCommandPool(factory.m_transferCommandPool)
            , m_transferQueue(factory.m_transferQueue) {
            factory.m_transferCommandPool = vk::CommandPool();
        }

        std::size_t resource_factory::create_pipeline(const std::string& techniqueName,
                                                      std::string_view pipelineName,
                                                      const std::string& shadersName,
                                                      const vertex_buffer& vertexBuffer) {
            auto shaders = m_device.m_resourceCache->find_shaders(shadersName);
            auto technique = m_device.m_resourceCache->find_technique(techniqueName);

            if (!shaders) {
                shaders = load_shaders(shadersName);
            }

            if (shaders && technique) {
                std::vector<vk::DescriptorSetLayout> layouts =
                    *m_device.m_resourceCache->find_descriptor_layouts(shadersName);

                pipeline pipeline(m_device.m_logicalDevice,
                                  m_device.m_engineSettings,
                                  base::parse_draw_settings(m_dataLoader, pipelineName),
                                  vertexBuffer.get_vertex_format(),
                                  *shaders,
                                  *technique,
                                  std::move(layouts),
                                  technique->get_extent());

                return m_device.m_resourceCache->add_pipeline(std::move(pipeline));
            }
            throw std::runtime_error("Can't create pipeline");
        }

        void resource_factory::create_technique(std::string&& name) {
            if (m_device.m_resourceCache->find_technique(name) != nullptr) {
                return;
            }

            m_device.m_resourceCache->add_rendering_technique(std::move(name),
                                                              base::parse_technique_settings(m_dataLoader, name),
                                                              m_device.m_logicalDevice,
                                                              *m_device.m_swapChain);
        }

        const shader_set* resource_factory::load_shaders(const std::string& shadersName) {
            auto shaders = m_shaderCompiler->compile_shaders(shadersName);

            m_device.m_resourceCache->add_shaders(shadersName, std::move(shaders));

            return m_device.m_resourceCache->find_shaders(shadersName);
        }

        buffer::creation_info resource_factory::create_buffer_creation_info() const noexcept {
            return buffer::creation_info{m_device.m_logicalDevice,
                                         m_transferQueue,
                                         m_transferCommandPool,
                                         m_device.m_physicalDevice->get_memory_properties()};
        }

        uniform_buffer::creation_info resource_factory::create_uniform_creation_info(
            const std::string& shaderName, base::resource_bind_point bindPoint) const noexcept {
            const auto descriptorSets = m_device.m_resourceCache->find_descriptor_sets(shaderName, bindPoint);
            assert(descriptorSets);
            return uniform_buffer::creation_info{create_buffer_creation_info(), *descriptorSets, m_device.m_resourceIndex};
        }

        texture::creation_info resource_factory::create_texture_creation_info(const std::string& textureName) const {
            const auto textureFile = m_dataLoader.find_file(std::filesystem::path("textures") / (textureName));
            if (!textureFile) {
                throw std::runtime_error(fmt::format("Wrong texture path: no such file: %s", "textures/" + textureName));
            }
            return texture::creation_info{create_buffer_creation_info(),
                                          *m_device.m_resourceCache,
                                          vk::BufferUsageFlagBits::eTransferSrc,
                                          vk::MemoryPropertyFlagBits::eHostVisible |
                                              vk::MemoryPropertyFlagBits::eHostCoherent,
                                          m_dataLoader.load_image(textureFile.value()),
                                          m_device.m_resourceIndex};
        }

        material::creation_info resource_factory::create_material_creation_info(const std::string& shaderName,
                                                                                const std::vector<std::string>& textureNames,
                                                                                std::uint32_t staticStorageSize,
                                                                                std::uint32_t dynamicStorageSize) const {
            std::vector<texture::creation_info> textureInfos;
            textureInfos.reserve(textureNames.size());
            for (const auto& name : textureNames) {
                textureInfos.emplace_back(create_texture_creation_info(name));
            }
            const auto descriptorSets =
                m_device.m_resourceCache->find_descriptor_sets(shaderName, base::resource_bind_point::material_static);
            assert(descriptorSets);

            return material::creation_info{
                std::move(textureInfos),
                *descriptorSets,
                staticStorageSize == 0 ?
                    std::optional<uniform_buffer::creation_info>() :
                    create_uniform_creation_info(shaderName, base::resource_bind_point::material_static),
                dynamicStorageSize == 0 ?
                    std::optional<uniform_buffer::creation_info>() :
                    create_uniform_creation_info(shaderName, base::resource_bind_point::material_dynamic),
                staticStorageSize,
                dynamicStorageSize,
                m_device.m_resourceIndex};
        }
    } // namespace vulkan

} // namespace engine
} // namespace tst
