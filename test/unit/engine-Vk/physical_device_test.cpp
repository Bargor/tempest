// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <engine-impl/physical_device.h>
#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class PhysicalDeviceFixture : public ::testing::Test {
        public:
            PhysicalDeviceFixture() {

            }

            void SetUp() {
            }

            void TearDown() {
            }
        };

        TEST_F(PhysicalDeviceFixture, PhysicalDeviceConstruction) {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst