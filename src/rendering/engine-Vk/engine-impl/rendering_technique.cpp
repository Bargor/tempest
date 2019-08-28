// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "rendering_technique.h"

#include "swap_chain.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vk::RenderPass create_render_pass(const vk::Device& device, vk::Format format) {
            vk::AttachmentDescription colorAttachment(vk::AttachmentDescriptionFlags(),
                                                      format,
                                                      vk::SampleCountFlagBits::e1,
                                                      vk::AttachmentLoadOp::eClear,
                                                      vk::AttachmentStoreOp::eStore,
                                                      vk::AttachmentLoadOp::eDontCare,
                                                      vk::AttachmentStoreOp::eDontCare,
                                                      vk::ImageLayout::eUndefined,
                                                      vk::ImageLayout::ePresentSrcKHR);

            vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

            vk::SubpassDescription subpass(
                vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachmentRef);

            vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL,
                                             0,
                                             vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                             vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                             vk::AccessFlags(),
                                             vk::AccessFlagBits::eColorAttachmentRead |
                                                 vk::AccessFlagBits::eColorAttachmentWrite);

            vk::RenderPassCreateInfo renderPassInfo(
                vk::RenderPassCreateFlags(), 1, &colorAttachment, 1, &subpass, 1, &dependency);

            return device.createRenderPass(renderPassInfo);
        }

        std::vector<vk::Framebuffer> create_framebuffers(vk::Device device,
                                                         vk::RenderPass renderPass,
                                                         const std::vector<vk::ImageView>& imageViews,
                                                         vk::Extent2D extent) {
            std::vector<vk::Framebuffer> framebuffers(imageViews.size());

            for (std::uint32_t i = 0; i < imageViews.size(); i++) {
                vk::ImageView attachments[] = {imageViews[i]};

                vk::FramebufferCreateInfo createInfo(
                    vk::FramebufferCreateFlags(), renderPass, 1, attachments, extent.width, extent.height, 1);

                framebuffers[i] = device.createFramebuffer(createInfo);
            }

            return framebuffers;
        }

        rendering_technique::rendering_technique(const std::string& name, vk::Device& device, swap_chain& swapChain)
            : base::rendering_technique(name)
            , m_device(device)
            , m_swapChain(swapChain)
            , m_extent(swapChain.get_extent())
            , m_renderPass(create_render_pass(device, swapChain.get_format()))
            , m_framebuffers(
                  create_framebuffers(device, m_renderPass, m_swapChain.get_image_views(), swapChain.get_extent())) {
        }

        rendering_technique::rendering_technique(rendering_technique&& technique)
            : base::rendering_technique(std::move(technique))
            , m_device(technique.m_device)
            , m_swapChain(technique.m_swapChain)
            , m_renderPass(technique.m_renderPass)
            , m_framebuffers(std::move(technique.m_framebuffers)) {
            m_renderPass = vk::RenderPass();
        }

        rendering_technique::~rendering_technique() {
            for (auto framebuffer : m_framebuffers) {
                m_device.destroyFramebuffer(framebuffer);
            }

            m_device.destroyRenderPass(m_renderPass);
        }

        vk::RenderPassBeginInfo rendering_technique::generate_render_pass_info() const {
            vk::ClearValue clearColor = vk::ClearValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

            vk::RenderPassBeginInfo renderPassInfo(
                m_renderPass, m_framebuffers[m_swapChain.get_image_index()], vk::Rect2D({0, 0}, m_extent), 1, &clearColor);

            return renderPassInfo;
        }

        technique_cache::technique_cache(vk::Device& device, swap_chain& swapChain)
            : m_device(device), m_swapChain(swapChain) {
        }

        void technique_cache::add_rendering_technique(const std::string& techniqueName) {
            m_techniques.emplace_back(rendering_technique(techniqueName, m_device, m_swapChain));
        }

        std::optional<const rendering_technique&> technique_cache::find_technique(const std::string& techniqueName) {
            auto technique =
                std::find_if(m_techniques.begin(), m_techniques.end(), [&](const rendering_technique& technique) {
                    if (technique.get_name() == techniqueName) {
                        return true;
                    }
                    return false;
                });

            if (technique != m_techniques.end()) {
                return *technique;
            }
            return std::nullopt;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
