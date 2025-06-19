#pragma once

#include "IDaedalus.hpp"
#include "core/Game.hpp"

namespace daedalus {
class Daedalus final : public atlas::core::Game, public IDaedalus {
  public:
    auto pre_start() -> void override {}
    auto start() -> void override;
    auto post_start() -> void override {}

    auto pre_shutdown() -> void override {}
    auto shutdown() -> void override;
    auto post_shutdown() -> void override {}

    [[nodiscard]] auto should_quit() const -> bool override;
};
} // namespace daedalus
