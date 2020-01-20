// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "rendering_technique.h"

#include "../swap_chain.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vk::RenderPass create_render_pass(vk::Device device, vk::Format format) {
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
                const vk::ImageView attachments[] = {imageViews[i]};

                const vk::FramebufferCreateInfo createInfo(
                    vk::FramebufferCreateFlags(), renderPass, 1, attachments, extent.width, extent.height, 1);

                framebuffers[i] = device.createFramebuffer(createInfo);
            }

            return framebuffers;
        }

        rendering_technique::rendering_technique(std::string&& techniqueName,
                                                 base::technique_settings&& techniqueSettings,
                                                 vk::Device device,
                                                 const swap_chain& swapChain)
            : base::rendering_technique(
                  std::move(techniqueName),
                  std::move(techniqueSettings),
                  core::extent<std::uint32_t>{swapChain.get_extent().width, swapChain.get_extent().height})
            , m_device(device)
            , m_swapChain(swapChain)
            , m_extent(m_swapChain.get().get_extent())
            , m_renderPass(create_render_pass(device, m_swapChain.get().get_format()))
            , m_framebuffers(create_framebuffers(
                  device, m_renderPass, m_swapChain.get().get_image_views(), m_swapChain.get().get_extent())) {
        }

        rendering_technique::rendering_technique(std::string&& techniqueName,
                                                 base::viewport_callback&& viewportCallback,
                                                 base::scissor_callback&& scissorCallback,
                                                 std::vector<base::color_blending_settings>&& framebufferBlending,
                                                 const base::global_blending_settings& globalBlending,
                                                 vk::Device device,
                                                 const swap_chain& swapChain)
            : base::rendering_technique(
                  std::move(techniqueName),
                  std::move(viewportCallback),
                  std::move(scissorCallback),
                  std::move(framebufferBlending),
                  globalBlending,
                  core::extent<std::uint32_t>{swapChain.get_extent().width, swapChain.get_extent().height})
            , m_device(device)
            , m_swapChain(swapChain)
            , m_extent(m_swapChain.get().get_extent())
            , m_renderPass(create_render_pass(device, m_swapChain.get().get_format()))
            , m_framebuffers(create_framebuffers(
                  device, m_renderPass, m_swapChain.get().get_image_views(), m_swapChain.get().get_extent())) {
        }

        rendering_technique::rendering_technique(rendering_technique&& technique) noexcept
            : base::rendering_technique(std::move(technique))
            , m_device(technique.m_device)
            , m_swapChain(technique.m_swapChain)
            , m_extent(technique.m_extent)
            , m_renderPass(technique.m_renderPass)
            , m_framebuffers(std::move(technique.m_framebuffers)) {
            technique.m_renderPass = vk::RenderPass();
        }

        rendering_technique::~rendering_technique() {
            destroy();
        }

        void rendering_technique::recreate_technique(const swap_chain& newSwapChain) {
            destroy();

            m_swapChain = newSwapChain;

            m_renderPass = create_render_pass(m_device, m_swapChain.get().get_format());
            m_framebuffers = create_framebuffers(
                m_device, m_renderPass, m_swapChain.get().get_image_views(), m_swapChain.get().get_extent());
            m_extent = m_swapChain.get().get_extent();
            m_viewportSettings = m_viewportSettingsCallback({m_extent.width, m_extent.height});
            m_scissor = m_scissorCallback({m_extent.width, m_extent.height});
        }

        vk::RenderPassBeginInfo rendering_technique::generate_render_pass_info(vk::CommandBuffer buffer,
                                                                               vk::SubpassContents contents) const {
            vk::ClearValue clearColor = vk::ClearValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

            vk::RenderPassBeginInfo renderPassInfo(m_renderPass,
                                                   m_framebuffers[m_swapChain.get().get_image_index()],
                                                   vk::Rect2D({0, 0}, m_extent),
                                                   1,
                                                   &clearColor);

            buffer.beginRenderPass(renderPassInfo, contents);

            return renderPassInfo;
        }

        void rendering_technique::destroy() {
            for (auto framebuffer : m_framebuffers) {
                m_device.destroyFramebuffer(framebuffer);
            }
            if (m_renderPass) {
                m_device.destroyRenderPass(m_renderPass);
            }
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
