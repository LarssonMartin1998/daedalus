#include "Daedalus.hpp"

#include <print>

namespace daedalus {
auto Daedalus::start() -> void { std::println("Daedalus::start()"); }
auto Daedalus::shutdown() -> void { std::println("Daedalus::shutdown()"); }

auto Daedalus::should_quit() const -> bool { return true; }
} // namespace daedalus
