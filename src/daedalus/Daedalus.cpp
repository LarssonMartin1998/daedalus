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
    double x{}, y{}, z{};
};

struct Velocity : Component<Velocity> {
    double x{}, y{}, z{};
};

struct Health : Component<Health> {
    int health{};
};

struct Test : Component<Test> {
    int lol{};
};

namespace daedalus {
auto Daedalus::start() -> void {
    auto& hephaestus = get_engine().get_module<hephaestus::Hephaestus>().get();
    constexpr size_t num_ents = 1500000;
    for (size_t i = 0; i < num_ents; ++i) {
        hephaestus.create_entity(Test{.lol = 42});
    }

    for (size_t i = 0; i < num_ents; ++i) {
        constexpr int vel = 1;
        hephaestus.create_entity(
            Transform{.x = 0.0, .y = 0.0, .z = 0.0},
            Velocity{.x = vel, .y = vel, .z = vel}
        );
    }

    constexpr int health = 5000;
    for (size_t i = 0; i < num_ents; ++i) {
        constexpr int vel = 10;
        hephaestus.create_entity(
            Transform{.x = 0.0, .y = 0.0, .z = 0.0},
            Velocity{.x = vel, .y = vel, .z = vel},
            Health{.health = health}
        );
    }

    hephaestus.create_entity(Health{.health = health});

    hephaestus.create_system([](const core::IEngine& engine,
                                std::tuple<Transform&, Velocity&> data) {
        const auto deltatime = engine.get_clock().get_delta_time();
        auto& [transform, velocity] = data;
        transform.x += velocity.x * deltatime;
        transform.y += velocity.y * deltatime;
        transform.z += velocity.z * deltatime;
    });

    hephaestus.create_system([](const core::IEngine& engine, std::tuple<Transform&> data) {
        auto& [transform] = data;
        std::println("T: {}, {}, {}", transform.x, transform.y, transform.z);
    });

    hephaestus.create_system([](const core::IEngine& engine, std::tuple<Test&> data) {
        auto& [test] = data;
        test.lol += 1;
    });

    std::println("Daedalus::start()");
}

auto Daedalus::shutdown() -> void {
    std::println("Daedalus::shutdown()");
}

auto Daedalus::should_quit() const -> bool {
    constexpr float game_lifetime = 1.F;
    return get_engine().get_clock().get_total_time() >= game_lifetime;
}
} // namespace daedalus
