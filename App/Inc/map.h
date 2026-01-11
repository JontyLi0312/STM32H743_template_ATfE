#ifndef __MAP_H
#define __MAP_H

#define PLACE_IN_RAM_SECTION __attribute__((section(".ram_section")))
#define PLACE_IN_LVGL_FB                                                       \
    __attribute__((section(".lvgl_fb"))) __attribute__((aligned(32)))
#define PLACE_IN_SDRAM_SECTION                                                 \
    __attribute__((section(".sdram_section"))) __attribute__((aligned(32)))
#define PLACE_IN_QSPI_SECTION __attribute__((section(".qspi_sectoin")))
#define PLACE_IN_LVGL_ASSETS  __attribute__((section(".lvgl_assets")))

#endif