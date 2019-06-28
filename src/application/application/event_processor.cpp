// This file is part of Tempest-application project
// Author: Karol Kontny

#include "event_processor.h"

#include <assert.h>

namespace tst {
namespace application {

    void event_processor::create_event(event&& event) noexcept {
        m_events[m_writeIndex++] = event;
        m_writeIndex &= m_mask;
        assert(m_writeIndex < m_queueSize);
    }

    void event_processor::subscribe(const std::size_t type,
                                    objectId id,
                                    std::function<void(const event::arguments&)>&& callback,
                                    std::chrono::duration<std::uint64_t, std::micro> updateFrequency) noexcept {
        subscriber subscriber{id, updateFrequency, std::move(callback)};
        m_listeners[type].emplace_back(subscriber);
    }

    void event_processor::process_events() {
        while (m_writeIndex != m_readIndex) { // it requires that number of events is less that queue size to work
            auto& event = m_events[m_readIndex++];
            m_readIndex &= m_mask;
            for (auto& listener : m_listeners[event.args.index()]) {
                if (event.id != listener.id) listener.callback(event.args);
            }
        }
    }

} // namespace application
} // namespace tst