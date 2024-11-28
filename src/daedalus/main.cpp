#include <memory>

#include "core/Engine.hpp"

#include "Daedalus.hpp"

auto main() -> int {
    auto engine = atlas::core::Engine{std::make_unique<daedalus::Daedalus>()};
    engine.run();

    return 0;
}
