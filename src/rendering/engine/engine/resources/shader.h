// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include "engine-impl/api.h"
#include "engine-impl/resources/shader.h"

namespace tst {
namespace engine {

    class shader final : private api::shader {
    public:
        using shader_type = base::shader_type;
    private:
    };

} // namespace engine
} // namespace tst
