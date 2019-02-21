// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include <memory>

namespace tst {
namespace application {
    class data_loader;
}

namespace engine {

    namespace scene {
        class scene;
    }

    class rendering_engine {
        template<typename T>
        using ptr = std::unique_ptr<T>;

    public:
        rendering_engine(application::data_loader& dataLoader);
        ~rendering_engine();

        void frame();
        void start();
        void stop();

    private:
        application::data_loader& m_dataLoader;
        ptr<scene::scene> m_scene;
    };

} // namespace engine
} // namespace tst