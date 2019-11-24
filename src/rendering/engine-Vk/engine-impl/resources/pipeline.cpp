// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "pipeline.h"

#include "rendering_technique.h"
#include "settings.h"
#include "vertex_format.h"

#include <assert.h>

namespace tst {
namespace engine {
    namespace vulkan {

        vk::PolygonMode translate_polygon_mode(base::rasterizer_settings::polygon_mode polygonMode) {
            switch (polygonMode) {
            case base::rasterizer_settings::polygon_mode::point:
                return vk::PolygonMode::ePoint;
            case base::rasterizer_settings::polygon_mode::line:
                return vk::PolygonMode::eLine;
            case base::rasterizer_settings::polygon_mode::fill:
                return vk::PolygonMode::eFill;
            }
            assert(false);
            return vk::PolygonMode::eFill;
        }

        vk::CullModeFlagBits translate_cull_mode(base::rasterizer_settings::cull_mode cullMode) {
            switch (cullMode) {
            case base::rasterizer_settings::cull_mode::back:
                return vk::CullModeFlagBits::eBack;
            case base::rasterizer_settings::cull_mode::front:
                return vk::CullModeFlagBits::eFront;
            case base::rasterizer_settings::cull_mode::front_and_back:
                return vk::CullModeFlagBits::eFrontAndBack;
            }
            assert(false);
            return vk::CullModeFlagBits();
        }

        vk::FrontFace translate_front_face(base::rasterizer_settings::front_face frontFace) {
            switch (frontFace) {
            case base::rasterizer_settings::front_face::clockwise:
                return vk::FrontFace::eClockwise;
            case base::rasterizer_settings::front_face::counter_clockwise:
                return vk::FrontFace::eCounterClockwise;
            }
            assert(false);
            return vk::FrontFace::eCounterClockwise;
        }

        vk::LogicOp translate_logic_operation(base::global_blending_settings::logic_operation logicOp) {
            switch (logicOp) {
            case base::global_blending_settings::logic_operation::clear:
                return vk::LogicOp::eClear;
            case base::global_blending_settings::logic_operation::and_op:
                return vk::LogicOp::eAnd;
            case base::global_blending_settings::logic_operation::and_reverse:
                return vk::LogicOp::eAndReverse;
            case base::global_blending_settings::logic_operation::copy:
                return vk::LogicOp::eCopy;
            case base::global_blending_settings::logic_operation::and_inverted:
                return vk::LogicOp::eAndInverted;
            case base::global_blending_settings::logic_operation::no_op:
                return vk::LogicOp::eNoOp;
            case base::global_blending_settings::logic_operation::xor_op:
                return vk::LogicOp::eXor;
            case base::global_blending_settings::logic_operation::or_op:
                return vk::LogicOp::eOr;
            case base::global_blending_settings::logic_operation::nor_op:
                return vk::LogicOp::eNor;
            case base::global_blending_settings::logic_operation::equivalent:
                return vk::LogicOp::eEquivalent;
            case base::global_blending_settings::logic_operation::invert:
                return vk::LogicOp::eInvert;
            case base::global_blending_settings::logic_operation::or_reverse:
                return vk::LogicOp::eOrReverse;
            case base::global_blending_settings::logic_operation::copy_inverted:
                return vk::LogicOp::eCopyInverted;
            case base::global_blending_settings::logic_operation::or_inverted:
                return vk::LogicOp::eOrInverted;
            case base::global_blending_settings::logic_operation::nand_op:
                return vk::LogicOp::eNand;
            case base::global_blending_settings::logic_operation::set:
                return vk::LogicOp::eSet;
            }
            assert(false);
            return vk::LogicOp::eClear;
        }

        vk::BlendOp translate_blend_operation(base::color_blending_settings::blend_operation blendOp) {
            switch (blendOp) {
            case base::color_blending_settings::blend_operation::add:
                return vk::BlendOp::eAdd;
            case base::color_blending_settings::blend_operation::subtract:
                return vk::BlendOp::eSubtract;
            case base::color_blending_settings::blend_operation::reverse_subtract:
                return vk::BlendOp::eReverseSubtract;
            case base::color_blending_settings::blend_operation::min:
                return vk::BlendOp::eMin;
            case base::color_blending_settings::blend_operation::max:
                return vk::BlendOp::eMax;
            }
            assert(false);
            return vk::BlendOp::eAdd;
        }

        vk::BlendFactor translate_blend_factor(base::color_blending_settings::blend_factor blendFactor) {
            switch (blendFactor) {
            case base::color_blending_settings::blend_factor::zero:
                return vk::BlendFactor::eZero;
            case base::color_blending_settings::blend_factor::one:
                return vk::BlendFactor::eOne;
            case base::color_blending_settings::blend_factor::src_color:
                return vk::BlendFactor::eSrcColor;
            case base::color_blending_settings::blend_factor::one_minus_src_color:
                return vk::BlendFactor::eOneMinusDstColor;
            case base::color_blending_settings::blend_factor::dst_color:
                return vk::BlendFactor::eDstColor;
            case base::color_blending_settings::blend_factor::one_minus_dst_color:
                return vk::BlendFactor::eOneMinusDstColor;
            case base::color_blending_settings::blend_factor::src_alpha:
                return vk::BlendFactor::eSrcAlpha;
            case base::color_blending_settings::blend_factor::one_minus_src_alpha:
                return vk::BlendFactor::eOneMinusSrcAlpha;
            case base::color_blending_settings::blend_factor::dst_alpha:
                return vk::BlendFactor::eDstAlpha;
            case base::color_blending_settings::blend_factor::one_minus_dst_alpha:
                return vk::BlendFactor::eOneMinusDstAlpha;
            case base::color_blending_settings::blend_factor::constant_color:
                return vk::BlendFactor::eConstantColor;
            case base::color_blending_settings::blend_factor::one_minus_constant_color:
                return vk::BlendFactor::eOneMinusConstantColor;
            case base::color_blending_settings::blend_factor::constant_alpha:
                return vk::BlendFactor::eConstantAlpha;
            case base::color_blending_settings::blend_factor::one_minus_constant_alpha:
                return vk::BlendFactor::eOneMinusConstantAlpha;
            case base::color_blending_settings::blend_factor::src_alpha_saturate:
                return vk::BlendFactor::eSrcAlphaSaturate;
            case base::color_blending_settings::blend_factor::src_1_color:
                return vk::BlendFactor::eSrc1Color;
            case base::color_blending_settings::blend_factor::one_minus_src_1_color:
                return vk::BlendFactor::eOneMinusSrc1Color;
            case base::color_blending_settings::blend_factor::src_1_alpha:
                return vk::BlendFactor::eSrc1Alpha;
            case base::color_blending_settings::blend_factor::one_minus_src_1_alpha:
                return vk::BlendFactor::eOneMinusSrc1Alpha;
            }
            assert(false);
            return vk::BlendFactor::eOne;
        }

        vk::ColorComponentFlags
        translate_color_component(base::color_blending_settings::color_component_flags colorWriteMask) {
            vk::ColorComponentFlags flags;
            if (colorWriteMask.r) {
                flags |= vk::ColorComponentFlagBits::eR;
            }
            if (colorWriteMask.g) {
                flags |= vk::ColorComponentFlagBits::eG;
            }
            if (colorWriteMask.b) {
                flags |= vk::ColorComponentFlagBits::eB;
            }
            if (colorWriteMask.a) {
                flags |= vk::ColorComponentFlagBits::eA;
            }
            return flags;
        }

        vk::PrimitiveTopology translate_primitive_topology(base::vertex_format::primitive_topology topology) {
            switch (topology) {
            case base::vertex_format::primitive_topology::line_list:
                return vk::PrimitiveTopology::eLineList;
            case base::vertex_format::primitive_topology::line_strip:
                return vk::PrimitiveTopology::eLineStrip;
            case base::vertex_format::primitive_topology::point_list:
                return vk::PrimitiveTopology::ePointList;
            case base::vertex_format::primitive_topology::triangle_fan:
                return vk::PrimitiveTopology::eTriangleFan;
            case base::vertex_format::primitive_topology::triangle_list:
                return vk::PrimitiveTopology::eTriangleList;
            case base::vertex_format::primitive_topology::triangle_strip:
                return vk::PrimitiveTopology::eTriangleStrip;
            }
            assert(false);
            return vk::PrimitiveTopology::eTriangleList;
        }

        vk::PipelineLayout create_pipeline_layout(const vk::Device logicalDevice,
                                                  const std::vector<vk::DescriptorSetLayout>& descriptorLayout) {
            vk::PipelineLayoutCreateInfo pipelineLayoutInfo(vk::PipelineLayoutCreateFlags(),
                                                            static_cast<std::uint32_t>(descriptorLayout.size()),
                                                            descriptorLayout.data(),
                                                            0,
                                                            nullptr);
            return logicalDevice.createPipelineLayout(pipelineLayoutInfo);
        }

        vk::PipelineVertexInputStateCreateInfo
        create_vertex_input_info(const vk::VertexInputBindingDescription& bindingDesc,
                                 const std::vector<vk::VertexInputAttributeDescription>& attributeDesc) {
            vk::PipelineVertexInputStateCreateInfo vertexInfo(vk::PipelineVertexInputStateCreateFlags(),
                                                              1,
                                                              &bindingDesc,
                                                              static_cast<std::uint32_t>(attributeDesc.size()),
                                                              attributeDesc.data());

            return vertexInfo;
        }

        vk::PipelineInputAssemblyStateCreateInfo create_assembly_info(const vertex_format& format) {
            vk::PipelineInputAssemblyStateCreateInfo assemblyInfo(
                vk::PipelineInputAssemblyStateCreateFlags(), translate_primitive_topology(format.get_topology()), false);

            return assemblyInfo;
        }

        vk::PipelineViewportStateCreateInfo create_viewport_info(const vk::Viewport& viewport, const vk::Rect2D& scissor) {
            vk::PipelineViewportStateCreateInfo viewportState(
                vk::PipelineViewportStateCreateFlags(), 1, &viewport, 1, &scissor);

            return viewportState;
        }

        vk::PipelineRasterizationStateCreateInfo create_rasterization_info(const base::rasterizer_settings& settings) {
            vk::PipelineRasterizationStateCreateInfo rasterizer(vk::PipelineRasterizationStateCreateFlags(),
                                                                settings.depthClamp,
                                                                settings.rasterizerDiscard,
                                                                translate_polygon_mode(settings.polygonMode),
                                                                translate_cull_mode(settings.cullMode),
                                                                translate_front_face(settings.frontFace),
                                                                settings.depthBias.enable,
                                                                settings.depthBias.constantFactor,
                                                                settings.depthBias.clamp,
                                                                settings.depthBias.slopeFactor,
                                                                settings.lineWidth);
            return rasterizer;
        } // namespace vulkan

        vk::PipelineMultisampleStateCreateInfo create_multisampling_info(const base::multisampling_settings& settings) {
            vk::SampleCountFlagBits sampleCount = static_cast<vk::SampleCountFlagBits>(settings.samples);

            vk::PipelineMultisampleStateCreateInfo multisampling(
                vk::PipelineMultisampleStateCreateFlags(), sampleCount, settings.enable, 1.0f, nullptr, false, false);

            return multisampling;
        }

        std::vector<vk::PipelineColorBlendAttachmentState>
        create_color_blend_attachment(const std::vector<base::color_blending_settings>& framebufferColorBlending) {
            std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachments;
            colorBlendAttachments.reserve(framebufferColorBlending.size());

            for (auto& attachment : framebufferColorBlending) {
                vk::PipelineColorBlendAttachmentState colorBlendAttachment(
                    attachment.enable,
                    translate_blend_factor(attachment.srcColorBlendFactor),
                    translate_blend_factor(attachment.dstColorBlendFactor),
                    translate_blend_operation(attachment.colorBlendOperation),
                    translate_blend_factor(attachment.srcAlphaBlendFactor),
                    translate_blend_factor(attachment.dstAlphaBlendFactor),
                    translate_blend_operation(attachment.alphaBlendOperation),
                    translate_color_component(attachment.colorWriteMask));
                colorBlendAttachments.emplace_back(colorBlendAttachment);
            }
            return colorBlendAttachments;
        }

        vk::PipelineColorBlendStateCreateInfo
        create_color_blending_info(base::global_blending_settings globalBlending,
                                   const std::vector<vk::PipelineColorBlendAttachmentState>& colorBlendAttachments) {
            vk::PipelineColorBlendStateCreateInfo colorBlending(
                vk::PipelineColorBlendStateCreateFlags(),
                globalBlending.enable,
                translate_logic_operation(globalBlending.logicalOperation),
                static_cast<std::uint32_t>(colorBlendAttachments.size()),
                colorBlendAttachments.data(),
                {globalBlending.blendConstants[0],
                 globalBlending.blendConstants[1],
                 globalBlending.blendConstants[2],
                 globalBlending.blendConstants[3]});

            return colorBlending;
        }

        vk::Pipeline compile_pipeline(const vk::Device logicalDevice,
                                      const vk::PipelineLayout pipelineLayout,
                                      const vk::RenderPass renderPass,
                                      const base::viewport_settings viewportSettings,
                                      const core::rectangle<std::int32_t, std::uint32_t> scissorSettings,
                                      const std::vector<base::color_blending_settings>& framebufferBlendingSettings,
                                      const base::global_blending_settings& globalBlendingSettings,
                                      const base::rasterizer_settings& rasterizerSettings,
                                      const base::multisampling_settings& multisamplingSettings,
                                      const vertex_format& format,
                                      const shader_set& shaders) {
            vk::Viewport viewport(static_cast<float>(viewportSettings.x),
                                  static_cast<float>(viewportSettings.y),
                                  static_cast<float>(viewportSettings.width),
                                  static_cast<float>(viewportSettings.height),
                                  viewportSettings.minDepth,
                                  viewportSettings.maxDepth);
            vk::Rect2D scissor({scissorSettings.offset.x, scissorSettings.offset.y},
                               {scissorSettings.dimensions.width, scissorSettings.dimensions.height});

            auto bindingDesc = format.get_binding_description();
            auto attributeDesc = format.get_attribute_descriptions();
            auto vertexInfo = create_vertex_input_info(bindingDesc, attributeDesc);
            auto assemblyInfo = create_assembly_info(format);
            auto viewportInfo = create_viewport_info(viewport, scissor);
            auto rasterizationInfo = create_rasterization_info(rasterizerSettings);
            auto multisamplingInfo = create_multisampling_info(multisamplingSettings);
            auto colorBlendAttachment = create_color_blend_attachment(framebufferBlendingSettings);
            auto blendingInfo = create_color_blending_info(globalBlendingSettings, colorBlendAttachment);

            std::vector<vk::PipelineShaderStageCreateInfo> shaderInfos;
            std::transform(shaders.shaders.cbegin(),
                           shaders.shaders.cend(),
                           std::back_inserter(shaderInfos),
                           [](const shader& shader) { return shader.get_pipeline_info(); });

            vk::GraphicsPipelineCreateInfo pipelineInfo(vk::PipelineCreateFlags(),
                                                        static_cast<std::uint32_t>(shaderInfos.size()),
                                                        shaderInfos.data(),
                                                        &vertexInfo,
                                                        &assemblyInfo,
                                                        nullptr,
                                                        &viewportInfo,
                                                        &rasterizationInfo,
                                                        &multisamplingInfo,
                                                        nullptr,
                                                        &blendingInfo,
                                                        nullptr,
                                                        pipelineLayout,
                                                        renderPass,
                                                        0,
                                                        vk::Pipeline(),
                                                        0);
            return logicalDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);
        }

        pipeline::pipeline(const vk::Device logicalDevice,
                           const settings& engineSettings,
                           const vertex_format& format,
                           const shader_set& shaders,
                           const rendering_technique& technique)
            : m_pipelineLayout(create_pipeline_layout(logicalDevice, shaders.layouts))
            , m_pipeline(compile_pipeline(logicalDevice,
                                          m_pipelineLayout,
                                          technique.m_renderPass,
                                          technique.m_viewportSettings,
                                          technique.m_scissor,
                                          technique.m_framebufferColorBlending,
                                          technique.m_globalColorBlending,
                                          engineSettings.m_rasterizer,
                                          engineSettings.m_multisampling,
                                          format,
                                          shaders))
            , m_pipelineSettings(technique.m_viewportSettings,
                                 technique.m_scissor,
                                 engineSettings.m_rasterizer,
                                 engineSettings.m_multisampling,
                                 technique.m_framebufferColorBlending,
                                 technique.m_globalColorBlending)
            , m_technique(technique)
            , m_shaders(shaders)
            , m_vertexFormat(format)
            , m_logicalDevice(logicalDevice) {
        }

        pipeline::pipeline(pipeline&& pipeline) noexcept
            : m_pipelineLayout(pipeline.m_pipelineLayout)
            , m_pipeline(pipeline.m_pipeline)
            , m_pipelineSettings(std::move(pipeline.m_pipelineSettings))
            , m_technique(pipeline.m_technique)
            , m_shaders(pipeline.m_shaders)
            , m_vertexFormat(pipeline.m_vertexFormat)
            , m_logicalDevice(pipeline.m_logicalDevice) {
            pipeline.m_pipeline = vk::Pipeline();
            pipeline.m_pipelineLayout = vk::PipelineLayout();
        }

        pipeline::~pipeline() {
            destroy();
        }

        void pipeline::bind_command_buffer(vk::CommandBuffer& buffer, vk::PipelineBindPoint bindPoint) const {
            buffer.bindPipeline(bindPoint, m_pipeline);
        }

        const rendering_technique& pipeline::get_technique() const noexcept {
            return m_technique;
        }

        void pipeline::recreate() {
            destroy();
            m_pipelineLayout = create_pipeline_layout(m_logicalDevice, m_shaders.layouts);
            m_pipelineSettings.m_viewport.width = m_technique.m_viewportSettings.width;
            m_pipelineSettings.m_viewport.height = m_technique.m_viewportSettings.height;

            m_pipeline = compile_pipeline(m_logicalDevice,
                                          m_pipelineLayout,
                                          m_technique.m_renderPass,
                                          m_pipelineSettings.m_viewport,
                                          m_pipelineSettings.m_scissor,
                                          m_pipelineSettings.m_framebufferColorBlending,
                                          m_pipelineSettings.m_globalColorBlending,
                                          m_pipelineSettings.m_rasterizer,
                                          m_pipelineSettings.m_multisampling,
                                          m_vertexFormat,
                                          m_shaders);
        }

        void pipeline::destroy() {
            if (m_pipeline) {
                m_logicalDevice.destroyPipeline(m_pipeline);
                m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
            }
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
