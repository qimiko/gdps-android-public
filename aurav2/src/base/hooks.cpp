#include "base/hooks.hpp"

void* GLOBAL_HANDLE = nullptr;
void* GLOBAL_BASE = nullptr;
size_t GLOBAL_PAGESIZE;

struct PageMapping {
    uintptr_t start;
    uintptr_t end;
    int32_t flags;
};

std::vector<PageMapping> STORED_MAPS;

bool create_hook(uintptr_t target_fn, uintptr_t hook_fn, uintptr_t o_fn)
{
    DobbyHook(reinterpret_cast<void*>(target_fn),
        reinterpret_cast<void*>(hook_fn), reinterpret_cast<void**>(o_fn));

    return true;
}

bool remove_hook(uintptr_t addr)
{
    DobbyDestroy(reinterpret_cast<void*>(addr));

    return true;
}

void setup_handle()
{
    Dl_info p;
    dladdr((void*)&AppDelegate::get, &p);

    GLOBAL_BASE = p.dli_fbase;

    auto handle = dlopen("libcocos2dcpp.so", RTLD_NOW);
    GLOBAL_HANDLE = handle;

    GLOBAL_PAGESIZE = sysconf(_SC_PAGESIZE);
}

uintptr_t get_global_base()
{
    return reinterpret_cast<uintptr_t>(GLOBAL_BASE);
}

void* get_sym_addr(const char* sym_name)
{
    if (GLOBAL_HANDLE == nullptr)
        return nullptr;

    return dlsym(GLOBAL_HANDLE, sym_name);
}

int32_t get_memory_protection_for_addr(uintptr_t addr)
{
    auto it = std::find_if(STORED_MAPS.begin(), STORED_MAPS.end(), [&addr](const auto& x) {
        return addr >= x.start && addr < x.end;
    });

    if (it != STORED_MAPS.end()) {
        return (*it).flags;
    }

    std::ifstream mapping("/proc/self/maps");
    std::string current_line;
    while (std::getline(mapping, current_line)) {
        uintptr_t start, end;
        char flags[4];

        std::sscanf(current_line.c_str(), "%x-%x %4c", &start, &end, flags);

        if (addr >= start && addr < end) {
            int32_t current_flags = 0;
            if (flags[0] == 'r') {
                current_flags |= PROT_READ;
            }

            if (flags[1] == 'w') {
                current_flags |= PROT_WRITE;
            }

            if (flags[2] == 'x') {
                current_flags |= PROT_EXEC;
            }

            PageMapping map_info = {
                start, end, current_flags
            };

            STORED_MAPS.push_back(map_info);

            return current_flags;
        }
    }

    return 0;
}

// https://stackoverflow.com/a/20382241
uintptr_t page_of(uintptr_t addr)
{
    return reinterpret_cast<uintptr_t>((addr) & ~(GLOBAL_PAGESIZE - 1));
}

bool patch_single_byte(uint8_t* addr, uint8_t byte)
{
    auto ptr_page = page_of(reinterpret_cast<uintptr_t>(addr));

    auto protection = get_memory_protection_for_addr(ptr_page);
    if (mprotect(reinterpret_cast<void*>(ptr_page), GLOBAL_PAGESIZE, PROT_WRITE) == 0) {
        std::memset(addr, byte, sizeof(byte));
        mprotect(reinterpret_cast<void*>(ptr_page), GLOBAL_PAGESIZE, protection);

        return true;
    } else {
        return false;
    }
}

bool patch_multiple_byte(uint8_t* addr, std::vector<uint8_t> bytes)
{
    auto ptr_page = page_of(reinterpret_cast<uintptr_t>(addr));
    auto end_ptr_page = page_of(reinterpret_cast<uintptr_t>(addr) + bytes.size());

    auto page_count = ((end_ptr_page - ptr_page) / GLOBAL_PAGESIZE) + 1;

    auto protection = get_memory_protection_for_addr(ptr_page);

    if (mprotect(reinterpret_cast<void*>(ptr_page), GLOBAL_PAGESIZE * page_count, PROT_WRITE) == 0) {
        std::memcpy(addr, bytes.data(), sizeof(decltype(bytes)::value_type) * bytes.size());
        mprotect(reinterpret_cast<void*>(ptr_page), GLOBAL_PAGESIZE * page_count, protection);

        return true;
    } else {
        return false;
    }
}