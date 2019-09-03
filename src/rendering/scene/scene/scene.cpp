#include "scene.h"

namespace tst {
namespace scene {
    std::vector<scene_object::state> update_scene(const scene& scene,
                                                  std::chrono::duration<std::uint64_t, std::micro> elapsedTime) {
        std::vector<scene_object::state> newSceneState;
        for (auto& object : scene.m_objects) {
            newSceneState.emplace_back(object.update_object(elapsedTime));
        }
        return newSceneState;
    }

    std::vector<engine::draw_info> prepare_draw_info(const std::vector<scene_object::state>& sceneState) {
        std::vector<engine::draw_info> drawInfos;
        drawInfos.reserve(sceneState.size());

        for (auto& state : sceneState) {
            engine::draw_info info(state.transformation, &(state.vertices[0]), &(state.indices[0]));
            drawInfos.emplace_back(std::move(info));
        }

        return drawInfos;
    }

    scene::scene() {
    }

    scene::~scene() {
    }

    void scene::add_object(scene_object&& object) {
        m_objects.emplace_back(std::move(object));
    }
} // namespace scene
} // namespace tst
