#include "base/module.hpp"

namespace Base {
void Module::initialize()
{
    if (!this->initialized_) {
        this->on_initialize();
    }
}
} // namespace Base