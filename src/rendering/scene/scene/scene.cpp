#include "scene.h"

#include "engine/resource_factory.h"
#include "object_controller.h"

namespace tst {
namespace scene {
    std::vector<scene_object::state> update_scene(scene& scene,
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

        for (const auto& state : sceneState) {
            engine::draw_info info(state.vertices, state.indices, state.pipeline, &state.uniform);
            drawInfos.emplace_back(std::move(info));
        }

        return drawInfos;
    }

    scene::scene(std::string&& sceneName,
                 application::data_loader& dataLoader,
                 application::event_processor<application::app_event>& eventProcessor,
                 engine::resource_factory& resourceFactory)
        : m_sceneName(std::move(sceneName))
        , m_sceneObjectController(std::make_unique<object_controller>(*this, dataLoader, eventProcessor, resourceFactory)) {
    }

    scene::~scene() {
    }

    void scene::add_object(scene_object&& object) {
        m_objects.emplace_back(std::move(object));
    }

    object_controller& scene::get_object_controller() const noexcept {
        return *m_sceneObjectController;
    }
} // namespace scene
} // namespace tst
