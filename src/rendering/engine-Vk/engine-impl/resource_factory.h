// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "api.h"
#include "resources/material.h"
#include "resources/texture.h"
#include "resources/uniform_buffer.h"

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {

    namespace vulkan {

        class device;
        class physical_device;
        class shader_compiler;
        class swap_chain;
        class vertex_buffer;
        class pipeline;
        struct shader_set;

        class resource_factory {
        public:
            resource_factory(const device& device, const application::data_loader& dataLoader);
            ~resource_factory();

            resource_factory(const resource_factory&) = delete;
            resource_factory(resource_factory&& factory) noexcept;

        public: // public resource factory interface
            std::size_t create_pipeline(const std::string& techniqueName,
                                        std::string_view pipelineName,
                                        const std::string& shadersName,
                                        const vertex_buffer& vertexBuffer);
            void create_technique(std::string&& name);

            material create_material(std::string&& materialName,
                                     const std::string& shaderName,
                                     const std::vector<std::string>& textureNames,
                                     std::uint32_t staticStorageSize,
                                     std::uint32_t dynamicStorageSize);

        public: // vulkan internal
            const shader_set* load_shaders(const std::string& shadersName);

            buffer::creation_info create_buffer_creation_info() const noexcept;
            uniform_buffer::creation_info create_uniform_creation_info(const std::string& shaderName,
                                                                       base::resource_bind_point bindPoint) const noexcept;
            texture::creation_info create_texture_creation_info(const std::string& textureName) const;

        private:
            const device& m_device;
            const application::data_loader& m_dataLoader;

            ptr<shader_compiler> m_shaderCompiler;
            vk::CommandPool m_transferCommandPool;
            vk::Queue m_transferQueue;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
