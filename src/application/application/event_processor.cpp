// This file is part of Tempest-application project
// Author: Karol Kontny

#include "event_processor.h"

#include <assert.h>

namespace tst {
namespace application {

    void event_processor::create_event(event&& event) noexcept {
        m_events[m_readIndex++] = event;
        m_readIndex &= m_mask;
        assert(m_readIndex < m_queueSize);
    }

    void event_processor::subscribe(const std::size_t type,
                                    std::function<void(const event::arguments&)>&& callback) noexcept {
        m_listeners[type].emplace_back(std::move(callback));
    }

    void event_processor::process_events() {
        while (m_writeIndex != m_readIndex) { // it requires that number of events is less that queue size to work
            auto& event = m_events[m_readIndex++];
            m_readIndex &= m_mask;
            for (auto& listener : m_listeners[event.args.index()]) {
                listener(event.args);
            }
        }
    }

} // namespace application
} // namespace tst