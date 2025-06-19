#include "Daedalus.hpp"

#include "core/IEngine.hpp"
#include "core/time/IEngineClock.hpp"

#include "hephaestus/Component.hpp"
#include "hephaestus/Hephaestus.hpp"
#include <print>
#include <tuple>

using namespace atlas;
using hephaestus::Component;

struct Transform : Component<Transform> {
    int x{}, y{}, z{};
};

struct Velocity : Component<Velocity> {
    int x{}, y{}, z{};
};

struct Health : Component<Health> {
    int health{};
};

namespace daedalus {
auto Daedalus::start() -> void {
    auto& hephaestus = get_engine().get_module<hephaestus::Hephaestus>().get();
    constexpr size_t num_ents = 100000;
    for (int i = 0; i < num_ents; ++i) {
        hephaestus.create_entity(Transform{.x = i, .y = i, .z = i});
    }
    for (int i = 0; i < num_ents; ++i) {
        hephaestus.create_entity(Transform{.x = i, .y = i, .z = i},
                                 Velocity{.x = i, .y = i, .z = i});
    }
    constexpr int health = 5000;
    for (int i = 0; i < num_ents; ++i) {
        hephaestus.create_entity(Transform{.x = i, .y = i, .z = i},
                                 Velocity{.x = i, .y = i, .z = i},
                                 Health{.health = health});
    }

    hephaestus.create_entity(Health{.health = health});

    hephaestus.create_system([](const core::IEngine& engine,
                                std::tuple<Transform&, Velocity&> data) {
        auto& [transform, velocity] = data;
        transform.x += velocity.x;
        transform.y += velocity.y;
        transform.z += velocity.z;
    });

    hephaestus.create_system(
        [](const core::IEngine& engine, std::tuple<Velocity&> data) {
            auto& [velocity] = data;
            velocity.x = 1;
            velocity.y = 1;
            velocity.z = 1;
        });

    hephaestus.create_system(
        [](const core::IEngine& engine, std::tuple<Transform&> data) {
            auto& [transform] = data;
            std::println("Transform: x={}, y={}, z={}", transform.x,
                         transform.y, transform.z);
        });

    hephaestus.create_system(
        [](const core::IEngine& engine, std::tuple<Health&> data) {
            auto& [health] = data;
            std::println("Health: {}", health.health);
        });

    hephaestus.create_system(
        [](const core::IEngine& engine, std::tuple<Velocity&> data) {
            auto& [velocity] = data;
            velocity.x = 1;
            velocity.y = 1;
            velocity.z = 1;
        });

    hephaestus.create_system(
        [](const core::IEngine& engine, std::tuple<Transform&> data) {
            auto& [transform] = data;
            std::println("Transform: x={}, y={}, z={}", transform.x,
                         transform.y, transform.z);
        });

    hephaestus.create_system(
        [](const core::IEngine& engine, std::tuple<Health&> data) {
            auto& [health] = data;
            std::println("Health: {}", health.health);
        });

    std::println("Daedalus::start()");
}

auto Daedalus::shutdown() -> void { std::println("Daedalus::shutdown()"); }

auto Daedalus::should_quit() const -> bool {
    constexpr float game_lifetime = 5.F;
    return get_engine().get_clock().get_total_time() >= game_lifetime;
    // return false;
}
} // namespace daedalus
