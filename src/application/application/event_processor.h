// This file is part of Tempest-application project
// Author: Karol Kontny

#pragma once

#include "object_id.h"
#include "time_source.h"

#include <array>
#include <assert.h>
#include <chrono>
#include <functional>
#include <variant>
#include <vector>

namespace tst {
namespace application {

    template<typename EventType>
    class event_processor {
    public:
        using Event = EventType;

    private:
        using hr_clock = std::chrono::high_resolution_clock;
        using event_id = std::size_t;

        friend class input_processor;

    public:
        event_processor(const time_source& timeSource);
        ~event_processor();

        void create_event(Event&& event) noexcept;

        void subscribe(const event_id type,
                       const objectId id,
                       std::function<void(const typename Event::arguments&)>&& callback,
                       std::chrono::duration<std::uint64_t, std::micro> updateFrequency =
                           std::chrono::microseconds::zero()) noexcept;
        void unsubscribe(const event_id type, const objectId id);
        void process_events();

    private:
        struct subscriber {
            objectId id;
            std::chrono::duration<std::uint64_t, std::micro> updateFrequency;
            core::time_point lastUpdateTime;
            std::function<void(const typename Event::arguments&)> callback;
        };

    private:
        static constexpr std::uint32_t m_queueSize = 256;
        static constexpr std::uint32_t m_mask = m_queueSize - 1;
        std::array<Event, m_queueSize> m_events; // keep size of queue aligned to power of 2, to have fast reset
        std::array<std::vector<subscriber>, std::variant_size_v<typename Event::arguments>> m_listeners;
        std::uint32_t m_readIndex;
        std::uint32_t m_writeIndex;
        const time_source& m_timeSource;
    };

    template<typename Event>
    event_processor<Event>::event_processor(const time_source& timeSource)
        : m_readIndex(0), m_writeIndex(0), m_timeSource(timeSource) {
    }

    template<typename Event>
    event_processor<Event>::~event_processor() {
    }

    template<typename Event>
    void event_processor<Event>::create_event(Event&& event) noexcept {
        m_events[m_writeIndex++] = event;
        m_writeIndex &= m_mask;
        assert(m_writeIndex < m_queueSize);
    }

    template<typename Event>
    void event_processor<Event>::subscribe(const event_id type,
                                           const objectId id,
                                           std::function<void(const typename Event::arguments&)>&& callback,
                                           std::chrono::duration<std::uint64_t, std::micro> updateFrequency) noexcept {
        subscriber subscriber{id, updateFrequency, m_timeSource.now(), std::move(callback)};
        m_listeners[type].emplace_back(subscriber);
    }

    template<typename Event>
    void event_processor<Event>::unsubscribe(const event_id type, const objectId id) {
        // TODO: replace with erase if
        m_listeners[type].erase(std::remove_if(m_listeners[type].begin(),
                                               m_listeners[type].end(),
                                               [id](const subscriber& subsciber) {
                                                   if (subsciber.id == id) return true;
                                                   return false;
                                               }),
                                m_listeners[type].end());
    }

    template<typename Event>
    void event_processor<Event>::process_events() {
        const auto now = m_timeSource.now();
        create_event(Event{this, typename Event::time{m_timeSource.get_time()}});

        while (m_writeIndex != m_readIndex) { // it requires that number of events is less that queue size to work
            const auto& event = m_events[m_readIndex++];
            m_readIndex &= m_mask;
            for (auto& listener : m_listeners[event.args.index()]) {
                if (event.id != listener.id) {
                    if (now - listener.lastUpdateTime >= listener.updateFrequency) {
                        listener.lastUpdateTime = now;
                        listener.callback(event.args);
                    }
                }
            }
        }
    }

} // namespace application
} // namespace tst