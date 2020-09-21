// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include "engine-base/view.h"
#include "resources/pipeline.h"

#include <util/flags.h>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class material;
        class index_buffer;
        class vertex_buffer;
        class uniform_buffer;

        struct draw_info {
            enum class bind_flag_bits : std::uint16_t {
                ePipeline = 0x0001,
                eGlobal_dynamic = 0x0002,
                eView_static = 0x0004,
                eView_dynamic = 0x0008,
                eMaterial_static = 0x0010,
                eMaterial_dynamic = 0x0020,
                eObject_static = 0x0040,
                eObject_dynamic = 0x0080,
                eDescriptor_sets = 0x00FE,
                eAll = 0xFFFF
            };

            using descritptor_set_bind_flags = core::flags<bind_flag_bits>;

        public:
            draw_info(const vertex_buffer& vertices,
                      const index_buffer* indices,
                      pipeline::pipeline_hash pipeline,
                      const base::view& view,
                      const material& material,
                      const std::vector<const uniform_buffer*>& uniforms);

            draw_info(const draw_info& other) = default;
            draw_info(draw_info&& other) noexcept;

            ~draw_info() = default;

        public:
            const vertex_buffer& vertices;
            const index_buffer* indices;
            const pipeline::pipeline_hash pipelineHash;
            const base::view viewData;
            const material& meshMaterial;
            std::vector<vk::DescriptorSet> descriptorSets;
            const pipeline* pipelineState;
            descritptor_set_bind_flags descriptorBindFlags;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
