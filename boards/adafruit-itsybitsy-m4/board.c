#include "board.h"
#include "periph/gpio.h"
#include "mtd_spi_nor.h"
#include "timex.h"

#ifdef MODULE_MTD
/* GD25x16 */
static const mtd_spi_nor_params_t _samd51_nor_params = {
    .opcode = &mtd_spi_nor_opcode_default,
    .wait_chip_erase = 15 * US_PER_SEC,
    .wait_32k_erase = 150 * US_PER_MS,
    .wait_64k_erase = 250 * US_PER_MS,
    .wait_sector_erase = 50 * US_PER_MS,
    .wait_chip_wake_up = 1 * US_PER_MS,
    .clk  = MHZ(54),
    .flag = SPI_NOR_F_SECT_4K
          | SPI_NOR_F_SECT_32K
          | SPI_NOR_F_SECT_64K,
    .spi  = SPI_DEV(1),
    .mode = SPI_MODE_0,
    .cs   = SAM0_QSPI_PIN_CS,
    .wp   = SAM0_QSPI_PIN_DATA_2,
    .hold = SAM0_QSPI_PIN_DATA_3,
};

static mtd_spi_nor_t samd51_nor_dev = {
    .base = {
        .driver = &mtd_spi_nor_driver,
        .page_size = 256,
        .pages_per_sector = 16,
    },
    .params = &_samd51_nor_params,
};

mtd_dev_t *mtd0 = (mtd_dev_t *)&samd51_nor_dev;
#endif /* MODULE_MTD */
