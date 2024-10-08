import kernel.base;
import kernel.lib;
import kernel.mm;

#include <boot/multiboot2.h>
#include <closureos/cpp_base.hpp>

/* temporarily here, remove sooon... */
extern "C" {
#include <boot/tty.h>
};

auto global_constructor_caller(void) -> int
{
    int (**init_array)(void) = &__init_array;
    int error;

    for (base::size_t i = 0; init_array[i]; i++) {
        error = init_array[i]();
        if (error) {
            return error;
        }
    }

    return 0;
}

auto main(multiboot_uint8_t *mbi) -> void
{
    mm::mm_core_init();

    if (global_constructor_caller() < 0) {
        boot_puts("[x] FAILED at invoking global constructors, hlting...");
        asm volatile ("hlt");
    }

    while (1) {
        boot_puts("[x] No work todo, hlting...");
        asm volatile ("hlt");
    }
}
