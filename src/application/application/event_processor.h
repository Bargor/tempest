// This file is part of Tempest-application project
// Author: Karol Kontny

#pragma once

#include "event_utils.h"

#include <array>
#include <chrono>
#include <functional>
#include <vector>

namespace tst {
namespace application {

    class event_processor {
        friend class input_processor;

    public:
        event_processor() = default;

        void create_event(event&& event) noexcept;
        void subscribe(const std::size_t type,
                       objectId id,
                       std::function<void(const event::arguments&)>&& callback,
                       std::chrono::duration<std::uint64_t, std::micro> updateFrequency =
                           std::chrono::microseconds::zero()) noexcept;
        void process_events();

    private:
        struct subscriber {
            objectId id;
            std::chrono::duration<std::uint64_t, std::micro> updateFrequency;
            std::function<void(const event::arguments&)> callback;
        };

    private:
        static constexpr std::uint32_t m_queueSize = 256;
        static constexpr std::uint32_t m_mask = m_queueSize - 1;
        std::array<event, m_queueSize> m_events; // keep size of queue aligned to power of 2, to have fast reset
        std::array<std::vector<subscriber>, std::variant_size_v<event::arguments>> m_listeners;
        std::uint32_t m_readIndex;
        std::uint32_t m_writeIndex;
    };
} // namespace application
} // namespace tst