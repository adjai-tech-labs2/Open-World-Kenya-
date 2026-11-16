#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <memory>

namespace kow::core {

struct Event { virtual ~Event() = default; };

struct EngineEvent : Event { std::string message; };
struct FrameBeginEvent : Event { std::uint64_t frame{0}; double dt{0.0}; };
struct FrameEndEvent : Event { std::uint64_t frame{0}; };
struct ModuleInitEvent : Event { std::string module; bool success{false}; };

class EventBus {
public:
    using HandlerId = std::uint64_t;

    template <typename E>
    HandlerId subscribe(std::function<void(const E&)> handler) {
        auto wrapper = [h = std::move(handler)](const Event& e) { h(static_cast<const E&>(e)); };
        auto ti = std::type_index(typeid(E));
        HandlerId id = next_id_++;
        handlers_[ti].push_back({id, std::move(wrapper)});
        return id;
    }

    template <typename E>
    void publish(const E& event) const {
        auto ti = std::type_index(typeid(E));
        auto it = handlers_.find(ti);
        if (it == handlers_.end()) return;
        for (auto& entry : it->second) entry.fn(event);
    }

    void clear() { handlers_.clear(); }
    [[nodiscard]] std::size_t handler_count() const {
        std::size_t n = 0; for (auto& [_, v] : handlers_) n += v.size(); return n;
    }

private:
    struct Entry { HandlerId id; std::function<void(const Event&)> fn; };
    std::unordered_map<std::type_index, std::vector<Entry>> handlers_;
    HandlerId next_id_{1};
};

} // namespace kow::core
