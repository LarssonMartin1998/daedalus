#pragma once

#include "IDaedalus.hpp"
#include "core/Game.hpp"

namespace daedalus {
class Daedalus final : public atlas::core::Game, public IDaedalus {
  public:
    auto start() -> void override;
    auto shutdown() -> void override;

    [[nodiscard]] auto should_quit() const -> bool override;
};
} // namespace daedalus
