// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

#include <engine-impl/api.h>
#include <engine-impl/draw_info.h>

namespace tst {
namespace engine {

    class draw_info final : public api::draw_info {
    public:
        using super = api::draw_info;

        ~draw_info() = default;

        draw_info(const resources::vertex_buffer* vertices,
                  const resources::index_buffer* indices,
                  const resources::pipeline& pipelineState,
                  const resources::uniform_buffer* uniforms);

        draw_info(api::draw_info&& impl);
        draw_info(draw_info&& other) noexcept;

    private:
    };

    static_assert(!std::is_polymorphic_v<draw_info>);
    static_assert(sizeof(draw_info) == sizeof(api::draw_info));

} // namespace engine
} // namespace tst
