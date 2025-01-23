#include <memory>

#include "core/Engine.hpp"

#include "Daedalus.hpp"

auto main() -> int {
    std::make_shared<atlas::core::Engine>(
        std::make_unique<daedalus::Daedalus>())
        ->run();

    return 0;
}
