#pragma once

#include "core/IGame.hpp"

namespace daedalus {
class IDaedalus : virtual public atlas::core::IGame {
  public:
    ~IDaedalus() override = default;

    IDaedalus(const IDaedalus &) = delete;
    auto operator=(const IDaedalus &) -> IDaedalus & = delete;

    IDaedalus(IDaedalus &&) = delete;
    auto operator=(IDaedalus &&) -> IDaedalus & = delete;

  protected:
    IDaedalus() = default;
};
} // namespace daedalus
