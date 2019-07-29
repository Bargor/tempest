// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/vertex_buffer.h"
#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"

#include <engine-impl/api.h>
#include <engine-impl/draw_info.h>

namespace tst {
namespace engine {

    class draw_info : public api::draw_info {
        public:
            using super = api::draw_info;

            draw_info(engine::api::uniform_buffer_object& transformation, const resources::vertex_buffer* vertices, const resources::index_buffer* indices);

        private:
            
    };

}
}