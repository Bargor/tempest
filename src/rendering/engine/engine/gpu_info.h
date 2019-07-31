// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/gpu_info.h>

namespace tst {
namespace engine {
    class gpu_info final : public api::gpu_info {
        public:
            using super = api::gpu_info;
            gpu_info(const api::gpu_info& info);
        private:
    };

    static_assert(!std::is_polymorphic_v<gpu_info>);
    static_assert(sizeof(gpu_info) == sizeof(api::gpu_info));
}
}