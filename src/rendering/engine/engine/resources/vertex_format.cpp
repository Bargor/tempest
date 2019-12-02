// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_format.h"

namespace tst {
namespace engine {

    vertex_format::vertex_format(base::vertex_format::primitive_topology topology) : api::vertex_format(topology) {
    }

    vertex_format::~vertex_format() {
        
    }

} // namespace engine
} // namespace tst