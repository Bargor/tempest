// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#if TST_COMPILER & TST_COMPILER_VC
#pragma warning(push)
#pragma warning(disable : 4324)
#endif

namespace tst {
    namespace engine {

        struct alignas(256) uniform_storage {
            uniform_storage() = default;
        };
    }
} // namespace tst

#if TST_COMPILER & TST_COMPILER_VC
#pragma warning(pop)
#endif