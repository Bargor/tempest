// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class instance {
            friend class engine;
            friend class device;

        public:
            static const instance& get_instance() noexcept;
            static std::vector<const char*> get_validation_layers() noexcept;
            static std::vector<std::string> get_supported_extensions() noexcept;
            inline static bool enableGlfwExtensions = true;

        public:
            const vk::Instance& get_instance_handle() const noexcept;

        private:
            instance(std::vector<const char*>&& requiredValidationLayers);
            ~instance();

        private:
            vk::Instance m_instance;
            vk::DebugUtilsMessengerEXT m_debugMessenger;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
