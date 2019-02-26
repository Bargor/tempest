// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "engine.h"

#include "scene/scene.h"

#include <fmt/printf.h>

namespace tst {
namespace engine {

    rendering_engine::rendering_engine(application::data_loader& dataLoader)
        : m_dataLoader(dataLoader), m_scene(std::make_unique<scene::scene>()) {
        backend::init_backend();
    }

    rendering_engine::~rendering_engine() {}

    void rendering_engine::frame() {}

} // namespace engine
} // namespace tst