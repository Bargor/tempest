// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/rendering_technique.h>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class rendering_technique : public base::rendering_technique {
        public:
            rendering_technique(const std::string& name,
                                vk::Device& device,
                                const std::vector<vk::ImageView>& imageViews,
                                vk::Format format,
                                vk::Extent2D extent);

            ~rendering_technique();

        private:
            vk::Device& m_device;
            vk::RenderPass m_renderPass;
            std::vector<vk::Framebuffer> m_framebuffers;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
