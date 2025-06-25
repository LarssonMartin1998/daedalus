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
    constexpr size_t NUM_ENTS = 2051250;
    for (size_t i = 0; i < NUM_ENTS; ++i) {
        hephaestus.create_entity(Test{.lol = 1});
    }

    for (size_t i = 0; i < NUM_ENTS; ++i) {
        constexpr int VEL = 1;
        hephaestus.create_entity(
            Transform{.x = 0.0, .y = 0.0, .z = 0.0},
            Velocity{.x = VEL, .y = VEL, .z = VEL}
        );
    }

    constexpr int HEALTH = 5000;
    for (size_t i = 0; i < NUM_ENTS; ++i) {
        constexpr int VEL = 10;
        hephaestus.create_entity(
            Transform{.x = 0.0, .y = 0.0, .z = 0.0},
            Velocity{.x = VEL, .y = VEL, .z = VEL},
            Health{.health = HEALTH}
        );
    }

    hephaestus.create_entity(Health{.health = HEALTH});

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
        // std::println("T: {}, {}, {}", transform.x, transform.y, transform.z);
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
    constexpr float GAME_LIFETIME = 5.F;
    return get_engine().get_clock().get_total_time() >= GAME_LIFETIME;
}
} // namespace daedalus
