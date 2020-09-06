// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include <tiny_obj_loader.h>

namespace tst {
namespace application {

    struct obj_data {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
    };

} // namespace application
} // namespace tst