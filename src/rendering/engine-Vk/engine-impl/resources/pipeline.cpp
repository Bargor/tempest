// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "pipeline.h"

#include "vertex_format.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vk::PipelineVertexInputStateCreateInfo create_vertex_input_info(const vertex_format& format) {
            vk::PipelineVertexInputStateCreateInfo vertexInfo(
                vk::PipelineVertexInputStateCreateFlags(),
                1,
                &format.get_binding_description(),
                static_cast<std::uint32_t>(format.get_attribute_descriptions().size()),
                format.get_attribute_descriptions().data());

            return vertexInfo;
        }

        vk::PipelineInputAssemblyStateCreateInfo create_assembly_info(const vertex_format& format) {
            vk::PipelineInputAssemblyStateCreateInfo assemblyInfo(
                vk::PipelineInputAssemblyStateCreateFlags(), format.get_topology(), false);

            return assemblyInfo;
        }

        vk::PipelineViewportStateCreateInfo create_viewport_info(
            base::viewport_settings& settings, core::rectangle<std::int32_t, std::uint32_t>& scissorSettings) {
            vk::Viewport viewport(settings.x,
                                  settings.y,
                                  static_cast<float>(settings.width),
                                  static_cast<float>(settings.height),
                                  settings.minDepth,
                                  settings.maxDepth);
            vk::Rect2D scissor({scissorSettings.offset.x, scissorSettings.offset.y},
                               {scissorSettings.dimensions.width, scissorSettings.dimensions.height});

            vk::PipelineViewportStateCreateInfo viewportState(
                vk::PipelineViewportStateCreateFlags(), 1, &viewport, 1, &scissor);

            return viewportState;
        }

        vk::Pipeline compile_pipeline(const vk::Device logicalDevice,
                                      const base::pipeline& base,
                                      const vertex_format& format,
                                      const shader_set& shaders,
                                      const rendering_technique& technique,
                                      const settings& settings) {
            auto vertexInfo = create_vertex_input_info(format);
            auto assemblyInfo = create_assembly_info(format);
            auto viewportInfo = create_viewport_info(base.m_viewport, base.m_scissor);
        }

        pipeline::pipeline(const vk::Device logicalDevice,
                           base::pipeline&& base,
                           const vertex_format& format,
                           const shader_set& shaders,
                           const rendering_technique& technique,
                           const settings& settings)
            : super(std::move(base))
            , m_pipeline(compile_pipeline(logicalDevice, *this, format, shaders, technique, settings))
            , m_technique(technique) {
        }

        pipeline::~pipeline() {
        }

        void pipeline::bind_command_buffer(vk::CommandBuffer& buffer, vk::PipelineBindPoint bindPoint) const {
            buffer.bindPipeline(bindPoint, m_pipeline);
        }

        const rendering_technique& pipeline::get_technique() const noexcept {
            return m_technique;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
