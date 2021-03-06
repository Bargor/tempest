// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "resources/shader.h"

#include <optional>

namespace tst {
namespace application {
    class data_loader;
}

namespace engine {
    namespace opengl {

        class shader_compiler {
        public:
            std::optional<shader_program> compile_program(std::string_view name,
                                                          const shaderTypesSet& types) const;

        public:
            shader_compiler(const application::data_loader& data_loader);

        private:
            std::optional<shader> compile_shader(const std::string& name, const opengl::shader_type type) const;

        private:
            const application::data_loader& m_dataLoader;
        };

    } // namespace opengl
} // namespace engine
} // namespace tst
