// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/uniform_buffer.h"

#include <engine-impl/api.h>
#include <engine-impl/draw_info.h>

namespace tst {
namespace engine {

    class mesh;
    class material;
    namespace resources {
        class pipeline;
    }

    class draw_info final : public api::draw_info {
        using super = api::draw_info;

    public:
        draw_info(const mesh& mesh,
                  const resources::pipeline& pipelineState,
                  const material& material,
                  const std::vector<const resources::uniform_buffer*>& uniforms);

        draw_info(const draw_info&) = delete;

        draw_info(api::draw_info&& impl);
        draw_info(draw_info&& other) noexcept;

        ~draw_info() = default;

    private:
    };

    static_assert(!std::is_polymorphic_v<draw_info>);
    static_assert(sizeof(draw_info) == sizeof(api::draw_info));

} // namespace engine
} // namespace tst
