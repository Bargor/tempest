// This file is part of Tempest-engine-Gl project
// Author: Karol Kontny

#include <engine/opengl_exception.h>
#include <engine/shader.h>

#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace opengl {

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

        TEST(Shader, ShaderConstruction) {
            try {
                shader shader(shader_type::vertex, "", "test_shader");
            } catch (opengl_exception&) {
                FAIL();
            }
        }

        TEST(Shader, ShaderMoveConstruction) {
            try {
                shader s(shader_type::vertex, "", "test_shader");
                EXPECT_TRUE(s.compile());
                shader s2(std::move(s));
                EXPECT_TRUE(s2.is_compiled());
            } catch (opengl_exception&) {
                FAIL();
            }
        }

        TEST(Shader, ShaderMoveAssigment) {
            try {
                shader s(shader_type::vertex, "", "test_shader");
                EXPECT_TRUE(s.compile());
                shader s2(shader_type::vertex, "", "test_shader_2");
                s = std::move(s2);
                EXPECT_TRUE(s2.is_compiled());
                EXPECT_FALSE(s.is_compiled());
            } catch (opengl_exception&) {
                FAIL();
            }
        }

        TEST(Shader, VertexShaderCompilationSuccess) {
            try {
                shader shader(shader_type::vertex, testVertexShaderCorrect, "test_shader");
                EXPECT_TRUE(shader.compile());
            } catch (opengl_exception&) {
                FAIL();
            }
        }

        TEST(Shader, VertexShaderCompilationFail) {
            try {
                shader shader(shader_type::vertex, testVertexShaderNotCorrect, "test_shader");
                EXPECT_FALSE(shader.compile());
            } catch (opengl_exception&) {
                FAIL();
            }
        }

        TEST(Shader, FragmentShaderCompilationSuccess) {
            try {
                shader shader(shader_type::fragment, testFragmentShaderCorrect, "test_shader");
                EXPECT_TRUE(shader.compile());
            } catch (opengl_exception&) {
                FAIL();
            }
        }

        TEST(Shader, FragmentShaderCompilationFail) {
            try {
                shader shader(shader_type::fragment, testFragmentShaderNotCorrect, "test_shader");
                EXPECT_FALSE(shader.compile());
            } catch (opengl_exception&) {
                FAIL();
            }
        }

        TEST(Program, ProgramConstruction) {
            try {
                shader_program program("test_program");
            } catch (opengl_exception&) {
                FAIL();
            }
        }

        TEST(Program, LinkingSuccess) {
            try {
                shader_program program("test_program");
                shader vertexShader(shader_type::vertex, testVertexShaderCorrect, "test_shader");
                shader fragmentShader(shader_type::fragment, testFragmentShaderCorrect, "test_shader");
                EXPECT_TRUE(vertexShader.compile());
                EXPECT_TRUE(fragmentShader.compile());
                program.attach_shader(vertexShader);
                program.attach_shader(fragmentShader);
                EXPECT_TRUE(program.link());
            } catch (opengl_exception&) {
                FAIL();
            }
        }

        TEST(Program, LinkingFail) {
            try {
                shader_program program("test_program");
                shader vertexShader(shader_type::vertex, testVertexShaderCorrect, "test_shader");
                EXPECT_FALSE(program.link());
            } catch (opengl_exception&) {
                FAIL();
            }
        }

    } // namespace opengl
} // namespace engine
} // namespace tst