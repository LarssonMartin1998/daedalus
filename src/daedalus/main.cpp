#include "core/Engine.hpp"

#include "Daedalus.hpp"

auto main() -> int {
    atlas::core::Engine<daedalus::Daedalus>{}.run();
    return 0;
}
