#include "Daedalus.hpp"

#include "core/IEngine.hpp"

#include "hephaestus/Hephaestus.hpp"
#include "hephaestus/IComponent.hpp"
#include "hephaestus/Query.hpp"
#include <print>

using namespace atlas;
using hephaestus::IComponent;

struct Transform : public IComponent {
    int x, y, z = 0;
};

struct Velocity : public IComponent {
    int x, y, z = 0;
};

namespace daedalus {
auto Daedalus::start() -> void {
    auto &hephaestus = get_engine().get_module<hephaestus::Hephaestus>().get();
    hephaestus.create_entity<Transform, Velocity>();

    hephaestus.create_system(
        [](const core::IEngine &engine,
           const hephaestus::Query<Transform, Velocity> &query) {
            for (const auto &[transform, velocity] : query.get()) {
                transform.x += velocity.x;
                transform.y += velocity.y;
                transform.z += velocity.z;
            }
        });

    hephaestus.create_system([](const core::IEngine &engine,
                                const hephaestus::Query<Velocity> &query) {
        for (const auto &[velocity] : query.get()) {
            velocity.x = 1;
            velocity.y = 1;
            velocity.z = 1;
        }
    });

    hephaestus.create_system([](const core::IEngine &engine,
                                const hephaestus::Query<Transform> &query) {
        for (const auto &[transform] : query.get()) {
            std::println("Transform: x={}, y={}, z={}", transform.x,
                         transform.y, transform.z);
        }
    });

    std::println("Daedalus::start()");
}

auto Daedalus::shutdown() -> void { std::println("Daedalus::shutdown()"); }

auto Daedalus::should_quit() const -> bool { return false; }
} // namespace daedalus
