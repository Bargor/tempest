// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#pragma once

#include <engine-base/resources/vertex_format.h>

namespace tst {
namespace engine {
    namespace opengl {
        class vertex_format : public base::vertex_format {
        public:
            using super = base::vertex_format;

            vertex_format();
            ~vertex_format();
        private:
        };
    } // namespace opengl
} // namespace engine
} // namespace tst