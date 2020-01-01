// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include <engine-base/vertex_format.h>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class vertex_format : public base::vertex_format {
        public:
            using super = base::vertex_format;

            vertex_format(super::primitive_topology topology);

            vk::VertexInputBindingDescription get_binding_description() const noexcept;
            std::vector<vk::VertexInputAttributeDescription> get_attribute_descriptions() const noexcept;

        private:
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
