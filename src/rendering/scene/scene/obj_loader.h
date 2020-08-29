// This file is part of Tempest-scene project
// Author: Karol Kontny
#pragma once

#include <engine/model.h>
#include <filesystem>
#include <string>

namespace tst {
namespace engine {
    class resource_factory;
}
namespace scene {
    engine::mesh load_obj_model(engine::resource_factory& factory, const std::filesystem::path& path);
}
} // namespace tst