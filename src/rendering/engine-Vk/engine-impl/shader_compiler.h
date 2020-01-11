// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/shader.h"

#include <filesystem>
#include <optional>
#include <rapidjson/document.h>

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {
    namespace vulkan {

        class device;
        class resource_cache;

        class shader_compiler {
        public:
            shader_compiler(const application::data_loader& dataLoader,
                            const vk::Device device,
                            resource_cache& resourceCache);
            ~shader_compiler();

            shader_set compile_shaders(const std::string& name) const;

        private:
            std::optional<std::vector<char>> load_bytecode(const std::optional<std::filesystem::path>& shaderSourceFile,
                                                           const std::optional<std::filesystem::path>& shaderBytecodeFile,
                                                           const std::string& bytecodeFileName) const;

        private:
            const application::data_loader& m_dataLoader;
            const vk::Device m_device;
            resource_cache& m_resourceCache;

            static constexpr char m_shaderExtension[] = ".spv";
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
