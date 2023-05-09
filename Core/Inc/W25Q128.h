#ifndef __W25Q128_H
#define __W25Q128_H

#ifdef __cplusplus
 extern "C" {
#endif

/**
  *  W25Q128 Configuration
  */
#define W25Q_FLASH_SIZE                  			0x1000000 /* 128 MBits => 16MBytes  256x64x1024*/
#define W25Q_SECTOR_SIZE                 			0x10000   /* 64x1024 - 256 sectors of 64KBytes */
#define W25Q_SUBSECTOR_SIZE              			0x1000    /* 4*1024 = 4096 subsectors of 4kBytes */
#define W25Q_PAGE_SIZE                   			0x100     /* 65536 pages of 256 bytes */

#define W25Q_DUMMY_CYCLES_READ_QUAD      			6

#define W25Q_AUTOPOLLING_INTERVAL_TIME   			0x10U

/**
 *  W25Q128 Commands W25Q Chip's Commands
 */

#define W25Q_WRITE_ENABLE_CMD 						0x06U	/* sets WEL bit, must be set before any write/program/erase */
#define W25Q_ENABLE_VOLATILE_SR_CMD 				0x50U	/* check 7.1 in data sheet */
#define W25Q_WRITE_DISABLE_CMD 						0x04U	/* resets WEL bit (state after power-up) */

#define W25Q_POWERUP_CMD 							0xABU	/* release power-down */
#define W25Q_FULLID_CMD 							0x90U	/* read Manufacturer ID & Device ID */
#define W25Q_READ_JEDEC_ID_CMD 						0x9FU	/* read JEDEC-standard ID */
#define W25Q_READ_UID_CMD 							0x4BU	/* read unique chip 64-bit ID */

#define W25Q_READ_DATA_CMD 							0x03U	/* read data by standard SPI */
#define W25Q_FAST_READ_CMD 							0x0BU	/* highest FR speed (8.2.12) */

#define W25Q_PAGE_PROGRAM_CMD 						0x02U	/* program page (256bytes) by single SPI line */

#define W25Q_SECTOR_ERASE_CMD 						0x20U	/* sets all 4Kbyte sector with 0xFF (erases it) */
#define W25Q_32KB_BLOCK_ERASE_CMD 					0x52U	/* sets all 32Kbyte block with 0xFF */
#define W25Q_64KB_BLOCK_ERASE_CMD 					0xD8U	/* sets all 64Kbyte block with 0xFF */
#define W25Q_CHIP_ERASE_CMD 						0xC7U	/* fill all the chip with 0xFF */

#define W25Q_READ_SR1_CMD 							0x05U	/* read status-register 1 */
#define W25Q_WRITE_SR1_CMD 							0x01U	/* write status-register 1 (8.2.5) */
#define W25Q_READ_SR2_CMD 							0x35U	/* read status-register 2 */
#define W25Q_WRITE_SR2_CMD 							0x31U	/* write status-register 2 (8.2.5) */
#define W25Q_READ_SR3_CMD 							0x15U	/* read ststus-register 3 */
#define W25Q_WRITE_SR3_CMD 							0x11U	/* write status-register 3 (8.2.5) */

#define W25Q_READ_SFDP_CMD 							0x5AU	/* read SFDP register parameters */
#define W25Q_ERASE_SECURITY_REG_CMD					0x44U	/* erase security registers */
#define W25Q_PROG_SECURITY_REG_CMD 					0x42U	/* program security registers */
#define W25Q_READ_SECURITY_REG_CMD 					0x48U	/* read security registers */

#define W25Q_GLOBAL_LOCK_CMD 						0x7EU	/* global read-only protection enable */
#define W25Q_GLOBAL_UNLOCK_CMD 						0x98U	/* global read-only protection disable */
#define W25Q_READ_BLOCK_LOCK_CMD 					0x3DU	/* check block/sector protection */
#define W25Q_IND_BLOCK_LOCK_CMD 					0x36U	/* make block/sector read-only */
#define W25Q_IND_BLOCK_UNLOCK_CMD 					0x39U	/* disable block/sector protection */

#define W25Q_ERASEPROG_SUSPEND_CMD					0x75U	/* suspend erase/program operation (can be applied only when SUS=0, BYSY=1) */
#define W25Q_ERASEPROG_RESUME_CMD 					0x7AU	/* resume erase/program operation (if SUS=1, BUSY=0) */
#define W25Q_POWERDOWN_CMD 							0xB9U	/* powers down the chip (power-up by reading ID) */

#define W25Q_ENABLE_RST_CMD 						0x66U	/* enable software-reset ability */
#define W25Q_RESET_CMD 								0x99U	/* make software reset */

#define W25Q_FAST_READ_DUAL_OUT_CMD 				0x3BU	/* fast read in dual-SPI OUTPUT (8.2.14) */
#define W25Q_FAST_READ_DUAL_IO_CMD 					0xBBU	/* fast read in dual-SPI I/O (address transmits by both lines) */
#define W25Q_FULLID_DUAL_IO_CMD 					0x92U	/* read Manufacturer ID & Device ID by dual I/O */
#define W25Q_PAGE_PROGRAM_QUAD_INP_CMD 				0x32U	/* program page (256bytes) by Quad SPI lines */
#define W25Q_FAST_READ_QUAD_OUT_CMD 				0x6BU	/* fast read in quad-SPI OUTPUT (8.2.16) */
#define W25Q_FULLID_QUAD_IO_CMD 					0x94U	/* read Manufacturer ID & Device ID by quad I/O */
#define W25Q_FAST_READ_QUAD_IO_CMD 					0xEBU	/* fast read in quad-SPI I/O (address transmits by quad lines) */
#define W25Q_SET_BURST_WRAP_CMD 					0x77U	/* use with quad-I/O (8.2.22) */

/**
  *   N25Q128A Registers
  */


/* Status Register */
#define W25Q_SR_Quad_Enable							((uint8_t)0x02)    /*!< Quad Enable */
#define W25Q_SR_DRV1                     			((uint8_t)0x9f)    /*!< DRV1:2 bit */


HAL_StatusTypeDef W25Q128_OCTO_SPI_Init(OSPI_HandleTypeDef* hospi);
HAL_StatusTypeDef W25Q128_OSPI_ResetChip(OSPI_HandleTypeDef* hospi);
HAL_StatusTypeDef W25Q128_OSPI_Configuration(OSPI_HandleTypeDef* hospi);
HAL_StatusTypeDef W25Q128_OSPI_WriteEnable(OSPI_HandleTypeDef* hospi);
HAL_StatusTypeDef W25Q128_OSPI_WriteDisable(OSPI_HandleTypeDef* hospi);
HAL_StatusTypeDef W25Q128_OSPI_AutoPollingMemReady(OSPI_HandleTypeDef* hospi);
HAL_StatusTypeDef W25Q128_OSPI_Erase_Chip(OSPI_HandleTypeDef* hospi);
HAL_StatusTypeDef W25Q128_OSPI_EraseSector(OSPI_HandleTypeDef* hospi, uint32_t EraseStartAddress, uint32_t EraseEndAddress);
HAL_StatusTypeDef W25Q128_OSPI_Erase_Block_32KB(OSPI_HandleTypeDef* hospi, uint32_t BlockAddress);
HAL_StatusTypeDef W25Q128_OSPI_Erase_Block_64KB(OSPI_HandleTypeDef* hospi, uint32_t BlockAddress);
HAL_StatusTypeDef W25Q128_OSPI_Write(OSPI_HandleTypeDef* hospi, uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
HAL_StatusTypeDef W25Q128_OSPI_Read(OSPI_HandleTypeDef* hospi, uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
HAL_StatusTypeDef W25Q128_OSPI_EnableMemoryMappedMode(OSPI_HandleTypeDef* hospi);
HAL_StatusTypeDef W25Q128_IsBusy(OSPI_HandleTypeDef* hospi);
HAL_StatusTypeDef W25Q128_Read_Status_Registers(OSPI_HandleTypeDef* hospi, uint8_t* register_data, uint8_t register_num);
HAL_StatusTypeDef W25Q128_Write_Status_Registers(OSPI_HandleTypeDef* hospi, uint8_t reg_data, uint8_t reg_num);

#ifdef __cplusplus
}
#endif

#endif
