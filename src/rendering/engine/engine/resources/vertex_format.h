// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/vertex_format.h>

#include <type_traits>

namespace tst {
namespace engine {

    class vertex_format final : public api::vertex_format {
    public:
        using super = api::vertex_format;

    public:
        vertex_format();
        ~vertex_format();
    private:
    };

    static_assert(sizeof(vertex_format) == sizeof(api::vertex_format));

} // namespace engine
} // namespace tst