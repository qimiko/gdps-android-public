#include "modules/limits.hpp"

namespace Limits {
void Module::on_initialize()
{
    std::vector<uint8_t> max_length = { 0x4B, 0xEA, 0x60, 0x60 };
    HookHandler::get_handler()
        .install_patch(0x1EAC14, max_length)
        .install_patch(0x1E3508, max_length)
        .install_patch(0x1E331C, max_length)
        .install_patch(0x1DABA8, max_length)
        .install_patch(0x1DA774, max_length)
        .install_patch(0x1EAC3C, { 0x04, 0xE0 })
        .install_patch(0x1EB6EC, { 0xC3, 0xE7 })
        .install_patch(0x23C720, { 0x09 })
        .install_patch(0x1F2ECC, { 0x09 });
}
} // namespace Limits