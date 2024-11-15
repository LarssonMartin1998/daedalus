#pragma once

#include "IDaedalus.hpp"

namespace daedalus {
class Daedalus final : public IDaedalus {
  public:
    auto start() -> void override;
    auto shutdown() -> void override;

    [[nodiscard]] auto should_quit() const -> bool override;
};
} // namespace daedalus
