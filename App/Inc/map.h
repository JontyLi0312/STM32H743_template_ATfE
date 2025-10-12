#ifndef MAP_H_
#define MAP_H_

#define QSPI_FLASH ".qspi_assets_section"
#define PLACE_IN_QSPI_SECTION                                                  \
    __attribute__((section(QSPI_FLASH))) __attribute__((used))

#endif