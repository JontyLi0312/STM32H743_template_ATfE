#ifndef MAP_H_
#define MAP_H_

#define PLACE_IN_BUFFER_SECTION __attribute__((section(".buffer")))

#define PLACE_IN_RAM_SECTION    __attribute__((section(".ram_section")))

#define PLACE_IN_QSPI_SECTION                                                  \
    __attribute__((section(".qspi_section"))) __attribute__((used))

#define PLACE_IN_SDRAM_SECTION                                                 \
    __attribute__((section(".sdram_section"))) __attribute__((aligned(32)))
#endif