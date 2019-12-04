// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/shader.h"

#include <filesystem>
#include <optional>
#include <string>
#include <vulkan/vulkan.hpp>

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {
    namespace vulkan {

        class device;

        class shader_compiler {
        public:
            shader_compiler(application::data_loader& data_loader, const device& device);
            ~shader_compiler();

            shader_set compile_shaders(const std::string& name) const;

        private:
            std::optional<std::vector<char>> load_bytecode(const std::optional<std::filesystem::path>& shaderSourceFile,
                                                           const std::optional<std::filesystem::path>& shaderBytecodeFile,
                                                           const std::string& bytecodeFileName) const;

        private:
            application::data_loader& m_dataLoader;
            const device& m_device;

            static constexpr char m_shaderExtension[] = ".spv";
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
