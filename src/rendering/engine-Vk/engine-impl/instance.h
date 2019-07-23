// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        class instance {
            friend class engine;
            friend class device;
        public:
            static instance& get_instance();
            static std::vector<const char*> get_validation_layers();

        public:
            vk::Instance& get_instance_handle();

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
