// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once
#include <core.h>
#include <string>

namespace tst {
namespace engine {
    namespace base {

        class rendering_technique {
        public:
            rendering_technique(std::string&& techniqueName);

            const std::string& get_name() const;

        protected:
            std::string m_techniqueName;
        };

        TST_INLINE const std::string& rendering_technique::get_name() const {
            return m_techniqueName;
        }
    } // namespace base
} // namespace engine
} // namespace tst

namespace std {} // namespace std
