// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/rendering_technique.h>
#include <functional>
#include <optional>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class swap_chain;

        class rendering_technique : public base::rendering_technique {
            friend class pipeline;

        public:
            rendering_technique(std::string&& techniqueName,
                                base::technique_settings&& techniqueSettings,
                                vk::Device device,
                                const swap_chain& swapChain);

            rendering_technique(std::string&& techniqueName,
                                base::viewport_callback&& viewportCallback,
                                base::scissor_callback&& scissorCallback,
                                const base::depth_settings& depthSettings,
                                const base::stencil_settings& stencilSettings,
                                std::vector<base::color_blending_settings>&& framebufferBlending,
                                const base::global_blending_settings& globalBlending,
                                vk::Device device,
                                const swap_chain& swapChain);

            rendering_technique(const rendering_technique& technique) = delete;
            rendering_technique(rendering_technique&& technique) noexcept;

            ~rendering_technique();

            void recreate_technique(const swap_chain& newSwapChain);

            vk::RenderPassBeginInfo generate_render_pass_info(vk::CommandBuffer buffer,
                                                              vk::SubpassContents contents) const;

        private:
            void destroy();

        private:
            vk::Device m_device;
            std::reference_wrapper<const swap_chain> m_swapChain;
            vk::Extent2D m_extent;
            vk::RenderPass m_renderPass;
            std::vector<vk::Framebuffer> m_framebuffers;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
