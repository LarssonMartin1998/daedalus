#pragma once

#include "IDaedalus.hpp"

namespace daedalus {
class Daedalus final : public IDaedalus {
  public:
    [[nodiscard]] auto should_quit() const -> bool override;
};
} // namespace daedalus
