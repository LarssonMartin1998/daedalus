#include "Daedalus.hpp"

#include "core/IEngine.hpp"
#include "core/time/IEngineClock.hpp"

#include "hephaestus/Component.hpp"
#include "hephaestus/Hephaestus.hpp"
#include <algorithm>
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

struct DeleteTimer : Component<DeleteTimer> {
    double elapsed{};
    double max_time{};
};

struct Damage : Component<Damage> {
    double value{};
    bool critical{false};
};

struct AI : Component<AI> {
    double intelligence{};
    int state{0};
};

struct Renderer : Component<Renderer> {
    int model_id{};
    double scale{1.0};
    bool visible{true};
};

struct Physics : Component<Physics> {
    double mass{};
    double friction{};
};

struct Lifetime : Component<Lifetime> {
    double max_time{};
    double elapsed{};
};

namespace daedalus {
auto Daedalus::start() -> void {
    auto& hephaestus = get_engine().get_module<hephaestus::Hephaestus>();

    constexpr std::size_t NUM_ENTS_PER_COMPONENT_SETUP = 2000000;

    for (std::size_t i = 0; i < NUM_ENTS_PER_COMPONENT_SETUP; ++i) {
        hephaestus.create_entity(Test{.lol = 1});
    }

    for (std::size_t i = 0; i < NUM_ENTS_PER_COMPONENT_SETUP; ++i) {
        constexpr int VEL = 1;
        hephaestus.create_entity(
            Transform{.x = 0.0, .y = 0.0, .z = 0.0},
            Velocity{.x = VEL, .y = VEL, .z = VEL}
        );
    }

    constexpr int HEALTH = 5000;
    for (std::size_t i = 0; i < NUM_ENTS_PER_COMPONENT_SETUP; ++i) {
        constexpr int VEL = 10;
        hephaestus.create_entity(
            Transform{.x = 0.0, .y = 0.0, .z = 0.0},
            Velocity{.x = VEL, .y = VEL, .z = VEL},
            Health{.health = HEALTH}
        );
    }

    constexpr std::size_t DELETE_COUNT = 1;
    for (std::size_t i = 0; i < DELETE_COUNT; ++i) {
        hephaestus.create_entity(DeleteTimer{.elapsed = 0.0, .max_time = 0.02});
    }

    hephaestus.create_entity(Health{.health = HEALTH});

    constexpr std::size_t NUM_PHYSICS_ENTITIES = 2000000;
    for (std::size_t i = 0; i < NUM_PHYSICS_ENTITIES; ++i) {
        hephaestus.create_entity(
            Transform{.x = 0.0, .y = 0.0, .z = 0.0},
            Physics{.mass = 10.0, .friction = 0.5}
        );
    }

    constexpr std::size_t NUM_RENDERER_ENTITIES = 2000000;
    for (std::size_t i = 0; i < NUM_RENDERER_ENTITIES; ++i) {
        hephaestus.create_entity(
            Transform{.x = 0.0, .y = 0.0, .z = 0.0},
            Renderer{.model_id = static_cast<int>(i % 100), .scale = 1.0, .visible = true}
        );
    }

    constexpr std::size_t NUM_AI_ENTITIES = 2000000;
    for (std::size_t i = 0; i < NUM_AI_ENTITIES; ++i) {
        hephaestus.create_entity(
            AI{.intelligence = 0.5 + (static_cast<double>(i % 10) / 10.0), .state = 0},
            Health{.health = HEALTH / 2}
        );
    }

    constexpr std::size_t NUM_DAMAGE_ENTITIES = 1000000;
    for (std::size_t i = 0; i < NUM_DAMAGE_ENTITIES; ++i) {
        hephaestus.create_entity(
            Damage{.value = 10.0 + (static_cast<double>(i % 5) * 2.0), .critical = (i % 10 == 0)},
            Lifetime{.max_time = 5.0, .elapsed = 0.0}
        );
    }

    constexpr std::size_t NUM_COMPLEX_ENTITIES = 1000000;
    for (std::size_t i = 0; i < NUM_COMPLEX_ENTITIES; ++i) {
        hephaestus.create_entity(
            Transform{.x = 0.0, .y = 0.0, .z = 0.0},
            Velocity{.x = 1.0, .y = 1.0, .z = 1.0},
            Health{.health = HEALTH},
            AI{.intelligence = 0.8, .state = 1},
            Renderer{.model_id = 200, .scale = 1.5, .visible = true}
        );
    }

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

    hephaestus.create_system([](const core::IEngine& engine,
                                std::tuple<const Transform&, Physics&> data) {
        const auto& [transform, physics] = data;
        const auto deltatime = engine.get_clock().get_delta_time();
        physics.friction = 0.5 + (std::abs(transform.x) * 0.01);
    });

    hephaestus.create_system([](const core::IEngine& engine,
                                std::tuple<const Transform&, const Renderer&> data) {
        const auto& [transform, renderer] = data;
        if (renderer.visible) {
        }
    });

    hephaestus.create_system([](const core::IEngine& engine, std::tuple<AI&, const Health&> data) {
        auto& [ai, health] = data;
        const auto deltatime = engine.get_clock().get_delta_time();

        if (health.health < 1000) {
            ai.state = 2;
        } else if (health.health < 3000) {
            ai.state = 1;
        } else {
            ai.state = 0;
        }

        ai.intelligence += deltatime * 0.01;
    });

    hephaestus.create_system([](const core::IEngine& engine,
                                std::tuple<Health&, const Damage&> data) {
        auto& [health, damage] = data;
        double damage_amount = damage.value;
        if (damage.critical) {
            damage_amount *= 2.0;
        }

        health.health = std::max(health.health - static_cast<int>(damage_amount), 0);
    });

    hephaestus.create_system([](const core::IEngine& engine, std::tuple<Lifetime&> data) {
        auto& [lifetime] = data;
        const auto deltatime = engine.get_clock().get_delta_time();
        lifetime.elapsed += deltatime;
    });

    static atlas::hephaestus::Entity destroy_entity = 100;
    hephaestus.create_system(
        [&hephaestus](const core::IEngine& engine, std::tuple<DeleteTimer&> data) {
            auto& [timer] = data;
            const auto& dt = engine.get_clock().get_delta_time();

            timer.elapsed += dt;
            if (timer.elapsed >= timer.max_time) {
                timer.elapsed = 0.0;
                timer.max_time += 0.02;

                hephaestus.destroy_entity(destroy_entity);
                destroy_entity += 80000;
            }
        }
    );
}

auto Daedalus::shutdown() -> void {}

auto Daedalus::should_quit() const -> bool {
    constexpr float GAME_LIFETIME = 5.F;
    if (const auto t = get_engine().get_clock().get_total_time_without_first_frame();
        t.has_value()) {
        return *t >= GAME_LIFETIME;
    }

    return false;
}
} // namespace daedalus
