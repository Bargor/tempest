// This file is part of Tempest-engine-Gl project
// Author: Karol Kontny

#include "application/data_loader.h"
#include "engine/shader_compiler.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;

namespace tst {
namespace engine {

    auto testVertexShaderCorrect = "#version 330\n \
                                       void main() \
                                       { \
                                           gl_Position = vec4(0.0, 0.0, 0.0, 0.0); \
                                       }";

    auto testVertexShaderNotCorrect = "#version 330\n \
                                            void main() \
                                            { \
                                                gl_Positionasd = vec4(0.0, 0.0, 0.0, 0.0); \
                                            }";

    auto testFragmentShaderCorrect = "#version 330\n \
                                         out vec4 fragColor; \
                                         void main() { fragColor = vec4(1.0, 0.0, 0.0, 1.0); }";

    auto testFragmentShaderNotCorrect = "#version 330\n \
                                         out vec3 fragColor; \
                                         void main() { fragColor = vec4(1.0, 0.0, 0.0, 1.0); }";

    class mock_data_loader : public application::data_loader {
    public:
        MOCK_CONST_METHOD2(load_shader_source, std::string(const std::string_view& name, const std::string_view& format));
    };

    TEST(ShaderCompiler, CompilingSuccessful) {
        mock_data_loader dataLoader;

        EXPECT_CALL(dataLoader, load_shader_source(std::string_view("test"), std::string_view("vs")))
            .WillRepeatedly(Return(testVertexShaderCorrect));
        EXPECT_CALL(dataLoader, load_shader_source(std::string_view("test"), std::string_view("fs")))
            .WillRepeatedly(Return(testFragmentShaderCorrect));

        auto compiler = shader_compiler(dataLoader);
        opengl::shaderTypesSet set;
        set[static_cast<std::size_t>(opengl::shader_type::vertex)] = true;
        set[static_cast<std::size_t>(opengl::shader_type::fragment)] = true;

        EXPECT_TRUE(compiler.compile_program("test", set));
    }

    TEST(ShaderCompiler, CompilingFail) {
        mock_data_loader dataLoader;

        EXPECT_CALL(dataLoader, load_shader_source(std::string_view("test"), std::string_view("vs")))
            .WillRepeatedly(Return(testVertexShaderCorrect));
        EXPECT_CALL(dataLoader, load_shader_source(std::string_view("test"), std::string_view("fs")))
            .WillRepeatedly(Return(testFragmentShaderNotCorrect));

        auto compiler = shader_compiler(dataLoader);
        opengl::shaderTypesSet set;
        set[static_cast<std::size_t>(opengl::shader_type::vertex)] = true;
        set[static_cast<std::size_t>(opengl::shader_type::fragment)] = true;

        EXPECT_FALSE(compiler.compile_program("test", set));
    }

    TEST(ShaderCompiler, LinkingFail) {
        mock_data_loader dataLoader;

        EXPECT_CALL(dataLoader, load_shader_source(std::string_view("test"), std::string_view("vs")))
            .WillRepeatedly(Return(""));
        EXPECT_CALL(dataLoader, load_shader_source(std::string_view("test"), std::string_view("fs")))
            .WillRepeatedly(Return(""));

        auto compiler = shader_compiler(dataLoader);
        opengl::shaderTypesSet set;
        set[static_cast<std::size_t>(opengl::shader_type::vertex)] = true;
        set[static_cast<std::size_t>(opengl::shader_type::fragment)] = true;

        EXPECT_FALSE(compiler.compile_program("test", set));
    }

} // namespace engine
} // namespace tst