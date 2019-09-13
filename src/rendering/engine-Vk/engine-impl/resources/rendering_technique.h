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
            rendering_technique(const std::string& name, vk::Device device, swap_chain& swapChain);

            rendering_technique(const rendering_technique& technique) = delete;
            rendering_technique(rendering_technique&& technique);

            ~rendering_technique();

            vk::RenderPassBeginInfo generate_render_pass_info() const;

        private:
            vk::Device& m_device;
            swap_chain& m_swapChain;
            vk::Extent2D m_extent;
            vk::RenderPass m_renderPass;
            std::vector<vk::Framebuffer> m_framebuffers;
        };

        class technique_cache {
        public:
            technique_cache(vk::Device& device, swap_chain& swapChain);

            void add_rendering_technique(const std::string& techniqueName);

            std::optional<std::reference_wrapper<rendering_technique>> find_technique(const std::string& techniqueName);

        private:
            vk::Device& m_device;
            swap_chain& m_swapChain;
            std::vector<rendering_technique> m_techniques;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
