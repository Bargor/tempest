// This file is part of Tempest-scene project
// Author: Karol Kontny
#pragma once

#include <engine/model.h>
#include <filesystem>
#include <string>

namespace tst {
namespace application {
    class data_loader;
}
namespace engine {
    class resource_factory;
}
namespace scene {
    engine::mesh load_obj_model(const application::data_loader& dataLoader,
                                engine::resource_factory& factory,
                                const std::filesystem::path& path);
}
} // namespace tst