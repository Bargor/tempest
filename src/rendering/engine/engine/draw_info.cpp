// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "draw_info.h"

namespace tst {
namespace engine {
    draw_info::draw_info(const resources::vertex_buffer* vertices,
                         const resources::index_buffer* indices,
                         const resources::pipeline& pipeline,
                         const resources::uniform_buffer* uniforms)
        : api::draw_info(vertices, indices, pipeline, uniforms) {
    }

    draw_info::draw_info(api::draw_info&& impl) : api::draw_info(std::move(impl)) {
    }

    draw_info::draw_info(draw_info&& other) noexcept : api::draw_info(std::move(other)) {
    }

} // namespace engine
} // namespace tst
