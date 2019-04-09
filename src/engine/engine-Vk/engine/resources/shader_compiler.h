// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "shader.h"

#include <optional>
#include <string>
#include <vulkan/vulkan.hpp>

namespace std {
namespace filesystem {
    class path;
} // namespace filesystem
} // namespace std

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {
    namespace vulkan {

        using shader_set = std::vector<shader>;

        class shader_compiler {
        public:
            shader_compiler(application::data_loader& data_loader, const vk::Device& device);
            ~shader_compiler();

            shader_set compile_shaders(const std::string& name) const;

        private:
            std::optional<std::vector<char>> load_bytecode(const std::optional<std::filesystem::path>& shaderSourceFile,
                                                           const std::optional<std::filesystem::path>& shaderBytecodeFile,
                                                           const std::string& bytecodeFileName) const;

        private:
            application::data_loader& m_dataLoader;
            const vk::Device& m_device;

            static constexpr char m_shaderExtension[] = ".spv";
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst