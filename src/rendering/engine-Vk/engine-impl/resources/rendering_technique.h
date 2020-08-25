// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/rendering_technique.h>
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

            rendering_technique(const rendering_technique& technique) = delete;
            rendering_technique(rendering_technique&& technique) noexcept;

            ~rendering_technique();

        public:
            void recreate_technique(const swap_chain& newSwapChain);

            vk::RenderPassBeginInfo generate_render_pass_info(vk::CommandBuffer buffer,
                                                              vk::SubpassContents contents) const;

            vk::Extent2D get_extent() const noexcept;

        private:
            void destroy();

        private:
            vk::Device m_device;
            std::reference_wrapper<const swap_chain> m_swapChain;
            vk::Extent2D m_extent;
            vk::RenderPass m_renderPass;
            std::vector<vk::Framebuffer> m_framebuffers;
        };

        TST_INLINE vk::Extent2D rendering_technique::get_extent() const noexcept {
            return m_extent;
        }

        TST_INLINE bool operator==(const rendering_technique& lhs, const rendering_technique& rhs) noexcept {
            return &lhs == &rhs;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
