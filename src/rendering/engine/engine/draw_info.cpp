// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "draw_info.h"

#include "material.h"
#include "mesh.h"
#include "resources/pipeline.h"

#include <util/cast.h>

namespace tst {
namespace engine {
    draw_info::draw_info(const mesh& mesh,
                         pipeline_hash pipeline,
                         const view& view,
                         const material& material,
                         const std::vector<const resources::uniform_buffer*>& uniforms)
        : api::draw_info(mesh.get_vertices().to_super(),
                         &mesh.get_indices()->to_super(),
                         pipeline,
                         view,
                         material.to_super(),
                         core::vector_cast<const api::uniform_buffer*>(uniforms)) {
    }

    draw_info::draw_info(api::draw_info&& impl) : api::draw_info(std::move(impl)) {
    }

    draw_info::draw_info(draw_info&& other) noexcept : api::draw_info(std::move(other)) {
    }

} // namespace engine
} // namespace tst
