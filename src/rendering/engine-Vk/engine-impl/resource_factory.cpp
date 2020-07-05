// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "device.h"
#include "physical_device.h"
#include "resource_cache.h"
#include "shader_compiler.h"
#include "swap_chain.h"

#include <application/data_loader.h>
#include <engine-base/pipeline_parser.h>
#include <engine-base/technique_parser.h>
#include <fmt/format.h>

namespace tst {
namespace engine {
    namespace vulkan {

        resource_factory::resource_factory(const device& device, const application::data_loader& dataLoader)
            : m_device(device)
            , m_dataLoader(dataLoader)
            , m_shaderCompiler(
                  std::make_unique<shader_compiler>(m_dataLoader, m_device.m_logicalDevice))
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

        template<>
        index_buffer<std::uint16_t> resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
            return index_buffer<std::uint16_t>(m_device.m_logicalDevice,
                                               m_transferQueue,
                                               m_transferCommandPool,
                                               m_device.m_physicalDevice->get_memory_properties(),
                                               vk::IndexType::eUint16,
                                               std::move(indices));
        }

        const pipeline& resource_factory::create_pipeline(const std::string& techniqueName,
                                                          const std::string& pipelineName,
                                                          const std::string& shadersName,
                                                          const vertex_format& format) {
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
                                  format,
                                  *shaders,
                                  *technique,
                                  std::move(layouts),
                                  technique->get_extent());

                auto hash = m_device.m_resourceCache->add_pipeline(std::move(pipeline));

                return *m_device.m_resourceCache->find_pipeline(hash);
            }
            throw std::runtime_error("Can't find pipeline");
        }

        void resource_factory::create_technique(const std::string& name) {
            if (m_device.m_resourceCache->find_technique(name) != nullptr) {
                return;
            }

            m_device.m_resourceCache->add_rendering_technique(rendering_technique(
                name, base::parse_technique_settings(m_dataLoader, name), m_device.m_logicalDevice, *m_device.m_swapChain));
        }

        vertex_buffer resource_factory::create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices) {
            return vertex_buffer(m_device.m_logicalDevice,
                                 m_transferQueue,
                                 m_transferCommandPool,
                                 m_device.m_physicalDevice->get_memory_properties(),
                                 format,
                                 std::move(vertices));
        }

        uniform_buffer resource_factory::create_uniform_buffer(const std::string& shaderName,
                                                               base::resource_bind_point bindPoint,
                                                               std::uint32_t binding,
                                                               std::size_t storageSize) {
            const auto descriptorSets = m_device.m_resourceCache->find_descriptor_sets(shaderName, bindPoint);
            assert(descriptorSets);
            return uniform_buffer(m_device.m_logicalDevice,
                                  m_transferQueue,
                                  m_transferCommandPool,
                                  *descriptorSets,
                                  binding,
                                  m_device.m_physicalDevice->get_memory_properties(),
                                  m_device.m_resourceIndex,
                                  storageSize);
        }

        texture resource_factory::create_texture(const std::string& textureName) {
            const auto textureFile = m_dataLoader.find_file(std::filesystem::path("textures") / (textureName));
            if (!textureFile) {
                throw std::runtime_error(fmt::format("Wrong texture path: no such file: %s", "textures/" + textureName));
            }
            return texture(m_device.m_logicalDevice,
                           m_transferQueue,
                           m_transferCommandPool,
                           *m_device.m_resourceCache,
                           vk::BufferUsageFlagBits::eTransferSrc,
                           m_device.m_physicalDevice->get_memory_properties(),
                           vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                           m_dataLoader.load_image(textureFile.value()),
                           m_device.m_resourceIndex);
        }

        const shader_set* resource_factory::load_shaders(const std::string& shadersName) {
            auto shaders = m_shaderCompiler->compile_shaders(shadersName);

            m_device.m_resourceCache->add_shaders(shadersName, std::move(shaders));

            return m_device.m_resourceCache->find_shaders(shadersName);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
