// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/shader.h>

#include <GL/glew.h>
#include <array>
#include <bitset>
#include <string>
#include <string_view>

namespace tst {
namespace engine {
    namespace opengl {

        using shader_type = base::shader_type;

        using shaderTypesSet = std::bitset<static_cast<std::size_t>(shader_type::enum_size)>;

        class shader {
            friend class shader_program;

        public:
            shader(shader_type type, std::string&& source, const std::string_view name);
            shader(const shader&) = delete;
            shader(shader&& shader);
            ~shader();

        public:
            shader& operator=(const shader&) = delete;
            shader& operator=(shader&& rhs);

        public:
            bool compile();
            inline bool is_compiled() const noexcept { return m_compiled; }

        private:
            using glShaderType = GLuint;

        private:
            bool check_compilation_status() const;

        private:
            shader_type m_type;
            glShaderType m_shaderId;
            std::string m_source;
            std::string_view m_name;
            mutable bool m_compiled;

            static constexpr std::array<std::uint32_t, static_cast<std::size_t>(shader_type::enum_size)>
                m_shaderTypesLookup = {{GL_VERTEX_SHADER,
                                        GL_FRAGMENT_SHADER,
                                        GL_GEOMETRY_SHADER,
                                        GL_COMPUTE_SHADER,
                                        GL_TESS_CONTROL_SHADER,
                                        GL_TESS_EVALUATION_SHADER}};
        };

        class shader_program {
        public:
            shader_program(const std::string_view name);
            ~shader_program();

            void attach_shader(const shader& shader);
            bool link();

        private:
            using glProgramType = GLuint;

        private:
            bool check_linking_status() const;
            void detach_shaders();

        private:
            glProgramType m_programId;
            std::string m_name;
            mutable bool m_linked;
        };

    } // namespace opengl
} // namespace engine
} // namespace tst