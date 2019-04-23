// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine.h"

#include "device_queue.h"
#include "resources/shader_compiler.h"
#include "scene/scene.h"
#include "swap_chain.h"
#include "vulkan_exception.h"

#include <application/event_processor.h>
#include <application/main_window.h>
#include <util/variant.h>

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

namespace tst {
namespace engine {

    static std::vector<const char*> generateLayers() {
        if (enableValidationLayers) {
            return {"VK_LAYER_LUNARG_standard_validation"};
        } else
            return {};
    }

    rendering_engine::rendering_engine(application::main_window& mainWindow,
                                       application::data_loader& dataLoader,
                                       application::event_processor& eventProcessor)
        : m_mainWindow(mainWindow)
        , m_dataLoader(dataLoader)
        , m_eventProcessor(eventProcessor)
        , m_scene(std::make_unique<scene::scene>())
        , m_requiredValidationLayers(generateLayers())
        , m_vulkanInstance(vulkan::init_Vulkan_instance(m_requiredValidationLayers, enableValidationLayers))
        , m_debugMessenger(vulkan::setup_debug_messenger(m_vulkanInstance, enableValidationLayers))
        , m_windowSurface(vulkan::create_window_surface(m_vulkanInstance, m_mainWindow.get_handle()))
        , m_physicalDevice(vulkan::select_physical_device(m_vulkanInstance, m_windowSurface, m_reqiuredDeviceExtensions))
        , m_queueIndices(vulkan::compute_queue_indices(m_physicalDevice, m_windowSurface))
        , m_logicalDevice(vulkan::create_logical_device(
              m_physicalDevice, m_queueIndices, m_requiredValidationLayers, m_reqiuredDeviceExtensions))
        , m_deviceQueues(std::make_unique<vulkan::device_queues>(m_logicalDevice, m_queueIndices))
        , m_swapChain(std::make_unique<vulkan::swap_chain>(m_physicalDevice,
                                                           m_logicalDevice,
                                                           m_windowSurface,
                                                           m_queueIndices,
                                                           mainWindow.get_size().width,
                                                           mainWindow.get_size().height))
        , m_shaderCompiler(std::make_unique<vulkan::shader_compiler>(m_dataLoader, m_logicalDevice))
        , m_renderPass(vulkan::create_render_pass(m_logicalDevice, m_swapChain->get_format()))
        , m_pipelineLayout(vulkan::create_pipeline_layout(m_logicalDevice))
        , m_pipeline(vulkan::create_graphics_pipeline(
              m_logicalDevice, m_pipelineLayout, m_renderPass, m_swapChain->get_extent(), *m_shaderCompiler.get()))
        , m_framebuffers(vulkan::create_framebuffers(
              m_logicalDevice, m_renderPass, m_swapChain->get_image_views(), m_swapChain->get_extent()))
        , m_commandPool(vulkan::create_command_pool(m_logicalDevice, m_queueIndices))
        , m_commandBuffers(vulkan::create_command_buffers(
              m_logicalDevice, m_commandPool, m_framebuffers, m_renderPass, m_pipeline, m_swapChain->get_extent()))
        , m_imageAvailable({m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo()),
                            m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo())})
        , m_renderFinished({m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo()),
                            m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo())})
        , m_inFlightFences({m_logicalDevice.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled)),
                            m_logicalDevice.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled))})
        , m_framebufferResized(false) {
        auto framebufferResizeCallback = [&](const application::event::arguments&) { m_framebufferResized = true; };
        m_eventProcessor.subscribe(core::variant_index<application::event::arguments, application::event::framebuffer>(),
                                   std::move(framebufferResizeCallback));
    }

    rendering_engine::~rendering_engine() {
        cleanup_swap_chain_dependancies();

        for (auto& semaphore : m_imageAvailable) {
            m_logicalDevice.destroySemaphore(semaphore);
        }
        for (auto& semaphore : m_renderFinished) {
            m_logicalDevice.destroySemaphore(semaphore);
        }
        for (auto& fence : m_inFlightFences) {
            m_logicalDevice.destroyFence(fence);
        }
        m_logicalDevice.destroyCommandPool(m_commandPool);
        m_swapChain.reset();
        m_vulkanInstance.destroySurfaceKHR(m_windowSurface);
        m_logicalDevice.destroy();
        if (enableValidationLayers) {
            vulkan::DestroyDebugUtilsMessengerEXT(m_vulkanInstance, m_debugMessenger, nullptr);
        }
        m_vulkanInstance.destroy();
    }

    void rendering_engine::cleanup_swap_chain_dependancies() {
        for (auto framebuffer : m_framebuffers) {
            m_logicalDevice.destroyFramebuffer(framebuffer);
        }

        m_logicalDevice.freeCommandBuffers(m_commandPool, m_commandBuffers);

        m_logicalDevice.destroyPipeline(m_pipeline);
        m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
        m_logicalDevice.destroyRenderPass(m_renderPass);
    }

    void rendering_engine::recreate_swap_chain(std::uint32_t width, std::uint32_t height) {
        m_logicalDevice.waitIdle();

        cleanup_swap_chain_dependancies();

        m_swapChain.reset();

        auto newSwapChain = std::make_unique<vulkan::swap_chain>(
            m_physicalDevice, m_logicalDevice, m_windowSurface, m_queueIndices, width, height);

        m_swapChain = std::move(newSwapChain);
        m_renderPass = vulkan::create_render_pass(m_logicalDevice, m_swapChain->get_format());
        m_pipelineLayout = vulkan::create_pipeline_layout(m_logicalDevice);
        m_pipeline = vulkan::create_graphics_pipeline(
            m_logicalDevice, m_pipelineLayout, m_renderPass, m_swapChain->get_extent(), *m_shaderCompiler.get());
        m_framebuffers = vulkan::create_framebuffers(
            m_logicalDevice, m_renderPass, m_swapChain->get_image_views(), m_swapChain->get_extent());
        m_commandBuffers = vulkan::create_command_buffers(
            m_logicalDevice, m_commandPool, m_framebuffers, m_renderPass, m_pipeline, m_swapChain->get_extent());
    }

    void rendering_engine::update_framebuffer() {
        m_framebufferResized = false;

        std::int32_t width, height;
        glfwGetFramebufferSize(m_mainWindow.get_handle(), &width, &height);
        m_mainWindow.set_size({width, height});
        recreate_swap_chain(width, height);
    }

    void rendering_engine::frame(size_t frameCount) {
        std::uint32_t currentFrame = frameCount % m_maxConcurrentFrames;

        m_logicalDevice.waitForFences(1, &m_inFlightFences[currentFrame], true, std::numeric_limits<uint64_t>::max());

        auto acquireResult = m_logicalDevice.acquireNextImageKHR(m_swapChain->get_native_swapchain(),
                                                                 std::numeric_limits<uint64_t>::max(),
                                                                 m_imageAvailable[currentFrame],
                                                                 vk::Fence());

        if (acquireResult.result == vk::Result::eErrorOutOfDateKHR) {
            update_framebuffer();
            return;
        } else if (acquireResult.result != vk::Result::eSuccess && acquireResult.result != vk::Result::eSuboptimalKHR) {
            throw vulkan::vulkan_exception("Failed to acquire image");
        }
        std::uint32_t imageIndex = acquireResult.value;

        vk::Semaphore waitSemaphores[] = {m_imageAvailable[currentFrame]};
        vk::Semaphore signalSemaphores[] = {m_renderFinished[currentFrame]};
        vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

        vk::SubmitInfo submitInfo(1, waitSemaphores, waitStages, 1, &m_commandBuffers[imageIndex], 1, signalSemaphores);

        m_logicalDevice.resetFences(1, &m_inFlightFences[currentFrame]);

        m_deviceQueues->m_graphicsQueueHandle.submit(1, &submitInfo, m_inFlightFences[currentFrame]);

        vk::PresentInfoKHR presentInfo(1, signalSemaphores, 1, &m_swapChain->get_native_swapchain(), &imageIndex);

        auto presentResult = m_deviceQueues->m_presentationQueueHandle.presentKHR(presentInfo);

        if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR ||
            m_framebufferResized) {
            update_framebuffer();
        } else if (presentResult != vk::Result::eSuccess) {
            throw vulkan::vulkan_exception("Failed to present image");
        }
    }

    void rendering_engine::stop() {
        m_logicalDevice.waitIdle();
    }

} // namespace engine
} // namespace tst