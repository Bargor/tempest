#include "scene.h"

#include "object_controller.h"

namespace tst {
namespace scene {
    std::vector<scene_object::state> update_scene(scene& scene,
                                                  std::chrono::duration<std::uint64_t, std::micro> elapsedTime) {
        std::vector<scene_object::state> newSceneState;
        for (auto& camera : scene.m_cameras) {
            camera.update(elapsedTime);
        }
        for (auto& object : scene.m_objects) {
            newSceneState.emplace_back(object.update_object(elapsedTime));
        }
        return newSceneState;
    }

    std::vector<engine::draw_info> prepare_draw_info(const camera& camera,
                                                     const std::vector<scene_object::state>& sceneState) {
        std::vector<engine::draw_info> drawInfos;
        drawInfos.reserve(sceneState.size());

        for (auto& state : sceneState) {
            engine::draw_info info(
                state.vertices, state.indices, state.pipeline, {&state.uniform, &camera.get_uniforms()}, {&state.texture});
            drawInfos.emplace_back(std::move(info));
        }

        return drawInfos;
    }

    scene::scene(std::string&& sceneName,
                 const application::data_loader& dataLoader,
                 application::event_processor<application::app_event>& eventProcessor,
                 engine::resource_factory& resourceFactory)
        : m_sceneName(std::move(sceneName))
        , m_objectController(std::make_unique<object_controller>(dataLoader, eventProcessor, resourceFactory)) {
    }

    scene::~scene() {
    }

    void scene::add_camera(const std::string& cameraName,
                           const glm::vec3& position,
                           const glm::vec3& lookAt,
                           const glm::vec3& up,
                           const float fov,
                           const float aspectRatio) {
        m_cameras.emplace_back(m_objectController->create_camera(cameraName, position, lookAt, up, fov, aspectRatio));
    }

    void scene::add_object(const std::string& objectName, const std::string& path) {
        m_objects.emplace_back(m_objectController->load_object(objectName, path));
    }

    camera& scene::get_camera(std::string_view cameraName) {
        for (auto& camera : m_cameras) {
            if (cameraName == camera.get_name()) {
                return camera;
            }
        }
        throw;
    }
} // namespace scene
} // namespace tst
