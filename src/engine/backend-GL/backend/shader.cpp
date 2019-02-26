// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "shader.h"

#include "api_exception.h"

#include <algorithm/algorithm.h>
#include <assert.h>
#include <fmt/printf.h>

namespace tst {
namespace engine {
    namespace backend {

        shader::shader(shader_type type, std::string&& source, const std::string_view name)
            : m_type(type)
            , m_shaderId(glCreateShader(m_shaderTypesLookup[static_cast<std::size_t>(type)]))
            , m_source(source)
            , m_name(name)
            , m_compiled(false) {
            if (!m_shaderId) {
                throw api_exception(fmt::format("Can't create shader, %s\n", name));
            }
        }

        shader::shader(shader&& shader)
            : m_type(shader.m_type)
            , m_shaderId(shader.m_shaderId)
            , m_source(std::move(shader.m_source))
            , m_name(shader.m_name)
            , m_compiled(shader.m_compiled) {
            shader.m_shaderId = 0;
        }

        shader::~shader() {
            if (m_shaderId) {
                glDeleteShader(m_shaderId);
            }
        }

        shader& shader::operator=(shader&& rhs) {
            assert(this != &rhs);
            std::swap(m_type, rhs.m_type);
            std::swap(m_shaderId, rhs.m_shaderId);
            std::swap(m_source, rhs.m_source);
            std::swap(m_name, rhs.m_name);
            std::swap(m_compiled, rhs.m_compiled);
            return *this;
        }

        bool shader::compile() {
            assert(!m_compiled);

            const char* shaderSources[1] = {&m_source[0]};
            const std::int32_t sourcesLength[1] = {static_cast<std::int32_t>(m_source.length())};
            glShaderSource(m_shaderId, 1, shaderSources, sourcesLength);
            glCompileShader(m_shaderId);
            return check_compilation_status();
        }

        bool shader::check_compilation_status() const {
            GLint isCompiled = 0;
            glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE) {
                GLint maxLength = 0;
                glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &maxLength);

                std::string errorLog;
                errorLog.resize(maxLength);
                glGetShaderInfoLog(m_shaderId, maxLength, &maxLength, &errorLog[0]);

                fmt::printf("Couldn't compile shader, %s,\n error: %s\n", m_name, errorLog);
                return false;
            }
            fmt::printf("Shader compilation %s succesful!\n", m_name);
            return m_compiled = true;
        }

        shader_program::shader_program(const std::string_view name)
            : m_programId(glCreateProgram()), m_name(name), m_linked(false) {
            if (!m_programId) {
                throw api_exception(fmt::format("Can't create program, %s\n", name));
            }
        }

        shader_program::~shader_program() { glDeleteProgram(m_programId); }

        void shader_program::attach_shader(const shader& shader) {
            assert(!m_linked);
            assert(shader.m_compiled);
            glAttachShader(m_programId, shader.m_shaderId);
        }

        void shader_program::detach_shaders() {
            assert(m_linked);
            GLint shadersCount;
            std::array<shader::glShaderType, shader_types_count> shaderIds;

            glGetAttachedShaders(m_programId, shader_types_count, &shadersCount, &shaderIds[0]);
            std::for_each_n(
                shaderIds.begin(), shadersCount, [&](auto shaderId) { glDetachShader(m_programId, shaderId); });
        }

        bool shader_program::link() {
            assert(!m_linked);
            glLinkProgram(m_programId);
            return check_linking_status();
        }

        bool shader_program::check_linking_status() const {
            GLint isLinked = 0;
            glGetProgramiv(m_programId, GL_LINK_STATUS, &isLinked);
            if (isLinked == GL_FALSE) {
                GLint maxLength = 0;
                glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

                std::string errorLog;
                errorLog.resize(maxLength);
                glGetProgramInfoLog(m_programId, maxLength, &maxLength, &errorLog[0]);
                fmt::printf("Couldn't link program, %s,\n error: %s\n", m_name, errorLog);

                return false;
            }
            fmt::printf("Program linking %s succesful!\n", m_name);
            m_linked = true;
            return true;
        }

    } // namespace backend
} // namespace engine
} // namespace tst