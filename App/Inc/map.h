#ifndef MAP_H_
#define MAP_H_

#define BUFFER                  ".buffer"
#define PLACE_IN_BUFFER_SECTION __attribute__((section(BUFFER)))

#define QSPI_FLASH              ".qspi_section"
#define PLACE_IN_QSPI_SECTION                                                  \
    __attribute__((section(QSPI_FLASH))) __attribute__((used))

#define SDRAM ".lvgl_buffers"
#define PLACE_IN_SDRAM_SECTION                                                 \
    __attribute__((section(SDRAM))) __attribute__((aligned(32)))

#endif