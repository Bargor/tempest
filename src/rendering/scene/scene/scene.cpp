#include "scene.h"

#include "object_controller.h"

#include <application/app_event.h>
#include <application/event_processor.h>
#include <engine/resource_factory.h>

namespace tst {
namespace scene {
    std::vector<scene_object::static_data*> update_scene(scene& scene,
                                                         std::chrono::duration<std::uint64_t, std::micro> elapsedTime) {
        std::vector<scene_object::static_data*> newSceneState;
        for (auto& camera : scene.m_cameras) {
            camera.update(elapsedTime);
        }
        for (auto& object : scene.m_objects) {
            object.update_object(elapsedTime);
            newSceneState.emplace_back(&object.get_static_data());
        }
        return newSceneState;
    }

    std::vector<engine::draw_info> prepare_draw_info(const camera& camera,
                                                     const std::vector<scene_object::static_data*>& sceneState) {
        std::vector<engine::draw_info> drawInfos;
        drawInfos.reserve(sceneState.size());

        for (auto& state : sceneState) {
            state->object.prepare_render_data(camera);
            engine::draw_info info(state->model.get_mesh(0),
                                   state->pipeline,
                                   camera.get_view(),
                                   state->model.get_material(0),
                                   {&state->uniform});
            drawInfos.emplace_back(std::move(info));
        }

        return drawInfos;
    }

    scene::scene(std::string&& sceneName,
                 const application::data_loader& dataLoader,
                 application::event_processor<application::app_event>& eventProcessor,
                 engine::resource_factory& resourceFactory)
        : m_sceneName(std::move(sceneName))
        , m_eventProcessor(eventProcessor)
        , m_resourceFactory(resourceFactory)
        , m_objectController(std::make_unique<object_controller>(dataLoader, m_resourceFactory)) {
    }

    void scene::add_camera(std::string cameraName,
                           const glm::vec3& position,
                           const glm::vec3& lookAt,
                           const glm::vec3& up,
                           const float fov,
                           const float aspectRatio) {
        m_cameras.emplace_back(
            std::move(cameraName), m_eventProcessor, position, lookAt, up, fov, aspectRatio);
    }

    void scene::add_object(std::string_view objectName, std::string_view path) {
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
