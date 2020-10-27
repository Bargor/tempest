// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "rendering_technique.h"

#include "../swap_chain.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vk::AttachmentLoadOp translate_attachment_load_operation(base::framebuffer_settings::load_operation loadOp) {
            switch (loadOp) {
            case base::framebuffer_settings::load_operation::load:
                return vk::AttachmentLoadOp::eLoad;
            case base::framebuffer_settings::load_operation::clear:
                return vk::AttachmentLoadOp::eClear;
            case base::framebuffer_settings::load_operation::dont_care:
                return vk::AttachmentLoadOp::eDontCare;
            }
            assert(false);
            return vk::AttachmentLoadOp::eDontCare;
        }

        vk::AttachmentStoreOp translate_attachment_store_operation(base::framebuffer_settings::store_operation storeOp) {
            switch (storeOp) {
            case base::framebuffer_settings::store_operation::store:
                return vk::AttachmentStoreOp::eStore;
            case base::framebuffer_settings::store_operation::dont_care:
                return vk::AttachmentStoreOp::eDontCare;
            }
            assert(false);
            return vk::AttachmentStoreOp::eDontCare;
        }

        vk::ImageLayout translate_image_layout(base::image_layout layout) {
            switch (layout) {
            case base::image_layout::undefined:
                return vk::ImageLayout::eUndefined;
            case base::image_layout::general:
                return vk::ImageLayout::eGeneral;
            case base::image_layout::color_attachment_optimal:
                return vk::ImageLayout::eColorAttachmentOptimal;
            case base::image_layout::depth_stencil_attachment_optimal:
                return vk::ImageLayout::eDepthStencilAttachmentOptimal;
            case base::image_layout::present:
                return vk::ImageLayout::ePresentSrcKHR;
            }
            assert(false);
            return vk::ImageLayout::eUndefined;
        }

        vk::PipelineBindPoint translate_bind_point(base::subpass_settings::bind_point bindPoint) {
            switch (bindPoint) {
            case base::subpass_settings::bind_point::graphics:
                return vk::PipelineBindPoint::eGraphics;
            case base::subpass_settings::bind_point::compute:
                return vk::PipelineBindPoint::eCompute;
            }
            assert(false);
            return vk::PipelineBindPoint::eGraphics;
        }

        std::vector<vk::AttachmentDescription> create_attachment_descriptions(
            const std::vector<base::framebuffer_settings>& framebufferSettings, vk::Format format, vk::Format depthFormat) {
            std::vector<vk::AttachmentDescription> attachments;
            attachments.reserve(framebufferSettings.size());

            for (const auto& framebuffer : framebufferSettings) {
                vk::AttachmentDescription attachment(
                    vk::AttachmentDescriptionFlags(),
                    framebuffer.type == base::framebuffer_settings::attachment_type::color ? format : depthFormat,
                    static_cast<vk::SampleCountFlagBits>(framebuffer.samples),
                    translate_attachment_load_operation(framebuffer.attachmentLoadOperation),
                    translate_attachment_store_operation(framebuffer.attachementStoreOperation),
                    translate_attachment_load_operation(framebuffer.stencilLoadOperation),
                    translate_attachment_store_operation(framebuffer.stencilStoreOperation),
                    translate_image_layout(framebuffer.initialLayout),
                    translate_image_layout(framebuffer.finalLayout));
                attachments.emplace_back(attachment);
            }

            return attachments;
        }

        std::vector<std::vector<vk::AttachmentReference>>
        create_attachment_references(const std::vector<base::subpass_settings>& subpassSettings) {
            std::vector<std::vector<vk::AttachmentReference>> references(subpassSettings.size());
            auto it = references.begin();
            for (const auto& subpass : subpassSettings) {
                for (const auto id : subpass.colorAttachments) {
                    it->emplace_back(vk::AttachmentReference(id, vk::ImageLayout::eColorAttachmentOptimal));
                }

                if (subpass.depthAttachment) {
                    it->emplace_back(vk::AttachmentReference(subpass.depthAttachment.value(),
                                                             vk::ImageLayout::eDepthStencilAttachmentOptimal));
                }
                ++it;
            }
            return references;
        }

        std::vector<vk::SubpassDescription>
        create_subpass_descriptions(const std::vector<base::subpass_settings>& subpassSettings,
                                    const std::vector<std::vector<vk::AttachmentReference>>& references) {
            std::vector<vk::SubpassDescription> subpasses;
            subpasses.reserve(subpassSettings.size());
            auto it = references.begin();
            for (const auto& subpass : subpassSettings) {
                const vk::AttachmentReference* depthAttachment =
                    subpass.depthAttachment.has_value() ? &(*(it->rbegin())) : nullptr;
                vk::SubpassDescription description(vk::SubpassDescriptionFlags(),
                                                   translate_bind_point(subpass.bindPoint),
                                                   0,
                                                   nullptr,
                                                   static_cast<std::uint32_t>(subpass.colorAttachments.size()),
                                                   it->data(),
                                                   nullptr,
                                                   depthAttachment);
                subpasses.emplace_back(description);
                ++it;
            }
            return subpasses;
        }

        std::vector<vk::SubpassDependency>
        create_subpass_dependencies(const std::vector<base::dependency>& dependenciesSettings) {
            std::vector<vk::SubpassDependency> dependencies;
            dependencies.reserve(dependenciesSettings.size());
            for (const auto& dependency : dependenciesSettings) {
                dependencies.emplace_back(
                    vk::SubpassDependency(dependency.srcSubpassId,
                                          dependency.dstSubpassId,
                                          static_cast<vk::PipelineStageFlags>(dependency.srcStageMask),
                                          static_cast<vk::PipelineStageFlags>(dependency.dstStageMask),
                                          static_cast<vk::AccessFlags>(dependency.srcAccessMask),
                                          static_cast<vk::AccessFlags>(dependency.dstAccessMask)));
            }
            return dependencies;
        }

        vk::RenderPass create_render_pass(vk::Device device,
                                          const base::technique_settings& settings,
                                          vk::Format format,
                                          vk::Format depthFormat) {
            const auto attachments = create_attachment_descriptions(settings.m_framebuffers, format, depthFormat);
            const auto references = create_attachment_references(settings.m_subpasses);
            const auto subpasses = create_subpass_descriptions(settings.m_subpasses, references);
            const auto dependencies = create_subpass_dependencies(settings.m_depencencies);

            vk::RenderPassCreateInfo renderPassInfo(vk::RenderPassCreateFlags(),
                                                    static_cast<std::uint32_t>(attachments.size()),
                                                    attachments.data(),
                                                    static_cast<std::uint32_t>(subpasses.size()),
                                                    subpasses.data(),
                                                    static_cast<std::uint32_t>(dependencies.size()),
                                                    dependencies.data());

            return device.createRenderPass(renderPassInfo);
        }

        std::vector<vk::Framebuffer> create_framebuffers(vk::Device device,
                                                         vk::RenderPass renderPass,
                                                         const std::vector<vk::ImageView>& imageViews,
                                                         vk::ImageView depthImageView,
                                                         vk::Extent2D extent,
                                                         bool useDepth) {
            std::vector<vk::Framebuffer> framebuffers(imageViews.size());

            for (std::uint32_t i = 0; i < imageViews.size(); i++) {
                const std::vector<vk::ImageView> attachments = useDepth ?
                    std::vector<vk::ImageView>{imageViews[i], depthImageView} :
                    std::vector<vk::ImageView>{imageViews[i]};

                const vk::FramebufferCreateInfo createInfo(vk::FramebufferCreateFlags(),
                                                           renderPass,
                                                           static_cast<std::uint32_t>(attachments.size()),
                                                           attachments.data(),
                                                           extent.width,
                                                           extent.height,
                                                           1);

                framebuffers[i] = device.createFramebuffer(createInfo);
            }

            return framebuffers;
        }

        rendering_technique::rendering_technique(std::string&& techniqueName,
                                                 base::technique_settings&& settings,
                                                 vk::Device device,
                                                 const swap_chain& swapChain)
            : base::rendering_technique(std::move(techniqueName), std::move(settings))
            , m_device(device)
            , m_swapChain(swapChain)
            , m_extent(m_swapChain.get().get_extent())
            , m_renderPass(create_render_pass(
                  device, m_settings, m_swapChain.get().get_format(), m_swapChain.get().get_depth_format()))
            , m_framebuffers(create_framebuffers(device,
                                                 m_renderPass,
                                                 m_swapChain.get().get_image_views(),
                                                 m_swapChain.get().get_depth_image_view(),
                                                 m_swapChain.get().get_extent(),
                                                 m_settings.m_useDepth)) {
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

            m_renderPass = create_render_pass(
                m_device, m_settings, m_swapChain.get().get_format(), m_swapChain.get().get_depth_format());
            m_framebuffers = create_framebuffers(m_device,
                                                 m_renderPass,
                                                 m_swapChain.get().get_image_views(),
                                                 m_swapChain.get().get_depth_image_view(),
                                                 m_swapChain.get().get_extent(),
                                                 m_settings.m_useDepth);
            m_extent = m_swapChain.get().get_extent();
        }

        vk::RenderPassBeginInfo rendering_technique::generate_render_pass_info(vk::CommandBuffer buffer,
                                                                               vk::SubpassContents contents) const {
            auto collectClearValues = [this]() {
                std::vector<vk::ClearValue> clearValues;
                clearValues.reserve(m_settings.m_framebuffers.size());
                for (const auto& framebuffer : m_settings.m_framebuffers) {
                    if (framebuffer.type == base::framebuffer_settings::attachment_type::color) {
                        clearValues.emplace_back(vk::ClearValue(framebuffer.clearValue));
                    } else {
                        clearValues.emplace_back(vk::ClearDepthStencilValue(
                            framebuffer.clearValue[0], static_cast<std::uint32_t>(framebuffer.clearValue[1])));
                    }
                }
                return clearValues;
            };

            const auto clearValues = collectClearValues();

            vk::RenderPassBeginInfo renderPassInfo(m_renderPass,
                                                   m_framebuffers[m_swapChain.get().get_image_index()],
                                                   vk::Rect2D({0, 0}, m_extent),
                                                   static_cast<std::uint32_t>(m_settings.m_framebuffers.size()),
                                                   clearValues.data());

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
