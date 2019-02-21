// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#pragma once

namespace tst {
namespace application {
    class data_loader;
}

namespace engine {
namespace scene {

    class scene {
    public:
        scene(application::data_loader& dataLoader);
        ~scene();

        void load_model();
    private:
        application::data_loader& m_dataLoader;
    };
} // namespace scene
} // namespace engine
} // namespace tst