#include "scene.h"

namespace tst {
namespace scene {
    scene update_scene(const scene& scene, std::chrono::duration<std::uint64_t, std::micro> elapsedTime) {
        return scene;
    }

    void scene::add_object(const scene_object& object) {
        m_objects.emplace_back(object);
    }
} // namespace scene
} // namespace tst
