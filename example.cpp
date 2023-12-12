extern "C" {
#include </opt/devkitpro/libtonc/include/tonc_tte.h>
#include </opt/devkitpro/libtonc/include/tonc_video.h>
#include </opt/devkitpro/libtonc/include/tonc_irq.h>
}

#include <string>
#include "filesystem.hpp"


void setup_debug_print()
{
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;

    tte_init_se(
                0,
                BG_CBB(0)|BG_SBB(31),
                0,
                CLR_YELLOW,
                14,
                NULL,
                NULL);
}



int main()
{
    setup_debug_print();

    if (!filesystem::is_mounted()) {
        tte_write("No filesystem appended!\n");
        while (1);
    }

    tte_write("walk fs:\n");
    filesystem::walk([](const char* path) {
                         tte_write(path);

                         auto contents = filesystem::load(path);
                         if (contents.first) {
                             tte_write(" size: ");
                             // Just using to_string for demo purposes, you
                             // shouldn't use this in real code.
                             tte_write(std::to_string(contents.second).c_str());
                         }

                         tte_write("\n");
                     });

    while (1);
}
