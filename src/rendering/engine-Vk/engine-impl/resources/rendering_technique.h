// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/rendering_technique.h>
#include <optional>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class swap_chain;

        class rendering_technique : public base::rendering_technique {
            friend class pipeline;

        public:
            rendering_technique(std::string&& name,
                                const base::viewport_settings& viewport,
                                const core::rectangle<std::int32_t, std::uint32_t> scissor,
                                std::vector<base::color_blending_settings> framebufferBlending,
                                const base::global_blending_settings& globalBlending,
                                vk::Device device,
                                const swap_chain& swapChain);

            rendering_technique(std::string&& techniqueName,
                                base::technique_settings&& techniqueSettings,
                                vk::Device device,
                                const swap_chain& swapChain);

            rendering_technique(const rendering_technique& technique) = delete;
            rendering_technique(rendering_technique&& technique) noexcept;

            ~rendering_technique();

            vk::RenderPassBeginInfo generate_render_pass_info() const;

        private:
            vk::Device m_device;
            const swap_chain& m_swapChain;
            vk::Extent2D m_extent;
            vk::RenderPass m_renderPass;
            std::vector<vk::Framebuffer> m_framebuffers;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
