#include "main.h"
#include "W25Q128.h"
#include "octospi.h"

/* OCTO SPI Initial Function */
HAL_StatusTypeDef W25Q128_OCTO_SPI_Init(OSPI_HandleTypeDef* hospi)
{
	if (HAL_OSPI_DeInit(hospi) != HAL_OK) {
	    return HAL_ERROR;
	}

	MX_OCTOSPI1_Init();

	if (W25Q128_OSPI_ResetChip(hospi) != HAL_OK) {
	    return HAL_ERROR;
	}
	if (W25Q128_OSPI_Configuration(hospi) != HAL_OK) {
        return HAL_ERROR;
    }
	HAL_Delay(1);
	if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK) {
        return HAL_ERROR;
    }
    if (W25Q128_OSPI_WriteEnable(hospi) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}

/* Reset Chip Function */
HAL_StatusTypeDef W25Q128_OSPI_ResetChip(OSPI_HandleTypeDef* hospi)
{
    OSPI_RegularCmdTypeDef sCommand={0};

    /* Enable Reset --------------------------- */
	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_ENABLE_RST_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Reset Device --------------------------- */
	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_RESET_CMD;							/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}

/* Enable Quad Mode & Set Dummy Cycles Count */
HAL_StatusTypeDef W25Q128_OSPI_Configuration(OSPI_HandleTypeDef* hospi)
{
    uint8_t reg1=0, reg2=0, reg3=0;
    uint8_t w_reg1=0, w_reg2=0, w_reg3=0;

    if (W25Q128_Read_Status_Registers(hospi, &reg1, 1) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (W25Q128_Read_Status_Registers(hospi, &reg2, 2) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (W25Q128_Read_Status_Registers(hospi, &reg3, 3) != HAL_OK)
    {
        return HAL_ERROR;
    }

    w_reg1 = reg1;
    w_reg2 = reg2 | W25Q_SR_Quad_Enable;
    w_reg3 = (reg3 & W25Q_SR_DRV1);

    if (W25Q128_Write_Status_Registers(hospi, w_reg1, 1) != HAL_OK)
    {
        return HAL_ERROR;
    }
    if (W25Q128_Write_Status_Registers(hospi, w_reg2, 2) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (W25Q128_Write_Status_Registers(hospi, w_reg3, 3) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/* Write Enable Function */
HAL_StatusTypeDef W25Q128_OSPI_WriteEnable(OSPI_HandleTypeDef* hospi)
{
    OSPI_RegularCmdTypeDef sCommand;
    OSPI_AutoPollingTypeDef sConfig;

    /* Enable write operations ------------------------------------------ */
	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_WRITE_ENABLE_CMD;					/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_READ_SR1_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;						/* Define Data Lines: Data On a Single Line */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 1;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match 					= 0x02U;
    sConfig.Mask 					= 0x02U;
    sConfig.MatchMode 				= HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval 				= W25Q_AUTOPOLLING_INTERVAL_TIME;
    sConfig.AutomaticStop 			= HAL_OSPI_AUTOMATIC_STOP_ENABLE;


    if (HAL_OSPI_AutoPolling(hospi, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/* Auto Polling Memory Function */
HAL_StatusTypeDef W25Q128_OSPI_AutoPollingMemReady(OSPI_HandleTypeDef* hospi)
{

    OSPI_RegularCmdTypeDef sCommand;
    OSPI_AutoPollingTypeDef sConfig;

    /* Configure automatic polling mode to wait for memory ready ------ */
	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_READ_SR1_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;						/* Define Data Lines: Data On a Single Line */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 1;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    sConfig.Match           			= 0x00U;
    sConfig.Mask            			= 0x01U;
    sConfig.MatchMode       			= HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval        			= W25Q_AUTOPOLLING_INTERVAL_TIME;
    sConfig.AutomaticStop   			= HAL_OSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_OSPI_AutoPolling(hospi, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/* Erase Chip Function */
HAL_StatusTypeDef W25Q128_OSPI_Erase_Chip(OSPI_HandleTypeDef* hospi)
{
    OSPI_RegularCmdTypeDef sCommand={0};
    //uint8_t reg3=0, w_reg3=0;

    /* Erasing Sequence ---------------------------------*/
	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_CHIP_ERASE_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 1;										/* Bytes Send With Data */

    if (W25Q128_OSPI_WriteEnable(hospi) != HAL_OK) {
        return HAL_ERROR;
    }

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    while (W25Q128_IsBusy(hospi)==HAL_ERROR)
    {
    	HAL_Delay(1);
    }

    if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/* Erase Sector Function */
HAL_StatusTypeDef W25Q128_OSPI_EraseSector(OSPI_HandleTypeDef* hospi, uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
    OSPI_RegularCmdTypeDef sCommand={0};
    uint32_t StartAddress=0;

    StartAddress = EraseStartAddress - (EraseStartAddress % W25Q_SECTOR_SIZE);

    while (EraseEndAddress >= StartAddress)
    {
    	/* Erasing Sequence -------------------------------------------------- */
    	/* Common Commands*/
    	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
    	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
    	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
    	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
    	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
    	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
    	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
    	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
    	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
    	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
    	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
    	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
    	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
    	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
    	/* Instruction */
    	sCommand.Instruction 				= W25Q_64KB_BLOCK_ERASE_CMD;				/* What We Do? */
    	/* Address */
    	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;					/* Define Address Lines: Address On a Single Line */
    	sCommand.Address					= (StartAddress & 0xFFFFFF);				/* Byte Address */
    	/* Data */
    	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
    	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
    	sCommand.NbData            			= 0;										/* Bytes Send With Data */

        if (W25Q128_OSPI_WriteEnable(hospi) != HAL_OK) {
            return HAL_ERROR;
        }

        if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            return HAL_ERROR;
        }

        if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK) {
            return HAL_ERROR;
        }

        StartAddress += W25Q_SECTOR_SIZE;
    }

    return HAL_OK;
}

/* Write Function */
HAL_StatusTypeDef W25Q128_OSPI_Write(OSPI_HandleTypeDef* hospi, uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
  OSPI_RegularCmdTypeDef sCommand={0};
  uint32_t end_addr=0, current_size=0, current_addr=0;
  uint32_t data_addr=0;

  current_addr = 0;

  while (current_addr <= WriteAddr) {
      current_addr += W25Q_PAGE_SIZE;
  }
  current_size = current_addr - WriteAddr;

  /* Check if the size of the data is less than the remaining place in the page */
  if (current_size > Size) {
      current_size = Size;
  }

  /* Initialize the adress variables */
  current_addr = WriteAddr;
  end_addr = WriteAddr + Size;

  data_addr = (uint32_t)pData;

  /* Perform the write page by page */
  do
  {
	/* Initialize the program command */
	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_PAGE_PROGRAM_QUAD_INP_CMD;			/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;					/* Define Address Lines: Address On a Single Line */
	sCommand.Address					= current_addr;								/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_4_LINES;					/* Define Data Lines: Data On Four Lines */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= current_size;								/* Bytes Send With Data */

    if (current_size == 0) {
        return HAL_OK;
    }

    /* Enable write operations */
    if (W25Q128_OSPI_WriteEnable(hospi) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Configure the command */
    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Transmission of the data */
    if (HAL_OSPI_Transmit(hospi, (uint8_t*)data_addr, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Configure automatic polling mode to wait for end of program */
    if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Update the address and size variables for next page programming */
    current_addr += current_size;
    data_addr += current_size;
    current_size = ((current_addr + W25Q_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q_PAGE_SIZE;
  } while (current_addr <= end_addr);

  return HAL_OK;
}

/* Read Function */
HAL_StatusTypeDef W25Q128_OSPI_Read(OSPI_HandleTypeDef* hospi,uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
  OSPI_RegularCmdTypeDef sCommand={0};
  /* Initialize the read command */
  /* Common Commands*/
  sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
  sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
  sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
  sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
  sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
  sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
  sCommand.SIOOMode          		= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
  sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
  sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
  sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
  sCommand.AlternateBytesDtrMode	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
  sCommand.InstructionMode   		= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
  sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
  sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
  /* Instruction */
  sCommand.Instruction 				= W25Q_FAST_READ_QUAD_IO_CMD;				/* What We Do? */
  /* Address */
  sCommand.AddressMode       		= HAL_OSPI_ADDRESS_4_LINES;					/* Define Address Lines: Address On Four Line */
  sCommand.Address					= ReadAddr;									/* Byte Address */
  /* Data */
  sCommand.DataMode          		= HAL_OSPI_DATA_4_LINES;					/* Define Data Lines: Data On Four Lines */
  sCommand.DummyCycles       		= W25Q_DUMMY_CYCLES_READ_QUAD;										/* Bytes Send With No Data */
  sCommand.NbData            		= Size;										/* Bytes Send With Data */

  /* Configure the command */
  if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_OSPI_Receive(hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/* Memory Map Enable Function */
HAL_StatusTypeDef W25Q128_OSPI_EnableMemoryMappedMode(OSPI_HandleTypeDef* hospi)
{

    OSPI_RegularCmdTypeDef sCommand={0};
    OSPI_MemoryMappedTypeDef sMemMappedCfg={0};

    /* Enable Memory-Mapped mode-------------------------------------------------- */
	/* Common Commands*/
    sCommand.OperationType      	= HAL_OSPI_OPTYPE_READ_CFG; 				/* Read Configuration (Memory-Mapped Mode) */
    sCommand.FlashId            	= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
    sCommand.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
    sCommand.AddressDtrMode     	= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
    sCommand.DataDtrMode			= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
    sCommand.DQSMode            	= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
    sCommand.SIOOMode          		= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
    sCommand.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
    sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
    sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
    sCommand.AlternateBytesDtrMode	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
    sCommand.InstructionMode   		= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
    sCommand.InstructionSize    	= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
    sCommand.AddressSize 			= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
    /* Instruction */
    sCommand.Instruction 			= W25Q_FAST_READ_QUAD_IO_CMD;				/* What We Do? */
    /* Address */
    sCommand.AddressMode       		= HAL_OSPI_ADDRESS_4_LINES;					/* Define Address Lines: Address On Four Lines */
    sCommand.Address				= 0;										/* Byte Address */
    /* Data */
    sCommand.DataMode          		= HAL_OSPI_DATA_4_LINES;					/* Define Data Lines: Data On Four Lines */
    sCommand.DummyCycles       		= W25Q_DUMMY_CYCLES_READ_QUAD;				/* Bytes Send With No Data */
    sCommand.NbData            		= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Initialize the program command */
	/* Common Commands*/
    sCommand.OperationType      	= HAL_OSPI_OPTYPE_WRITE_CFG; 				/* Write Configuration (Memory-Mapped Mode)) */
    sCommand.FlashId            	= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
    sCommand.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
    sCommand.AddressDtrMode     	= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
    sCommand.DataDtrMode			= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
    sCommand.DQSMode            	= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
    sCommand.SIOOMode          		= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
    sCommand.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
    sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
    sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
    sCommand.AlternateBytesDtrMode	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
    sCommand.InstructionMode   		= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
    sCommand.InstructionSize    	= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
    sCommand.AddressSize 			= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
    /* Instruction */
    sCommand.Instruction 			= W25Q_PAGE_PROGRAM_QUAD_INP_CMD;			/* What We Do? */
    /* Address */
    sCommand.AddressMode       		= HAL_OSPI_ADDRESS_1_LINE;					/* Define Address Lines: Address On a Single Line */
    sCommand.Address				= 0;										/* Byte Address */
    /* Data */
    sCommand.DataMode          		= HAL_OSPI_DATA_4_LINES;					/* Define Data Lines: Data On Four Lines */
    sCommand.DummyCycles       		= 0;										/* Bytes Send With No Data */
    sCommand.NbData            		= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Initialize Memory Mapped Command */
    sMemMappedCfg.TimeOutActivation 	= HAL_OSPI_TIMEOUT_COUNTER_DISABLE;		/* Timeout counter disabled, nCS remains active */

    if (HAL_OSPI_MemoryMapped(hospi, &sMemMappedCfg) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/* Check Chip is Busy Function */
HAL_StatusTypeDef W25Q128_IsBusy(OSPI_HandleTypeDef* hospi)
{
	HAL_StatusTypeDef state;
	uint8_t status_rgister = {0};

	state = W25Q128_Read_Status_Registers(hospi, &status_rgister, 1);
	if (state != HAL_OK)
		return state;

	status_rgister = status_rgister & 0b1;

	return status_rgister ? HAL_ERROR : HAL_OK;
}

/* Read Status Registers Function */
HAL_StatusTypeDef W25Q128_Read_Status_Registers(OSPI_HandleTypeDef* hospi, uint8_t* register_data, uint8_t register_num)
{
	OSPI_RegularCmdTypeDef sCommand={0};

	/* Common Commands*/
    sCommand.OperationType      	= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
    sCommand.FlashId            	= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
    sCommand.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
    sCommand.AddressDtrMode     	= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
    sCommand.DataDtrMode			= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
    sCommand.DQSMode            	= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
    sCommand.SIOOMode          		= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
    sCommand.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
    sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
    sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
    sCommand.AlternateBytesDtrMode	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
    sCommand.InstructionMode   		= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
    sCommand.InstructionSize    	= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
    sCommand.AddressSize 			= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
    /* Instruction */
    sCommand.Instruction 			= 0;										/* What We Do? */
    /* Address */
    sCommand.AddressMode       		= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
    sCommand.Address				= 0;										/* Byte Address */
    /* Data */
    sCommand.DataMode          		= HAL_OSPI_DATA_1_LINE;						/* Define Data Lines: Data On a Single Line */
    sCommand.DummyCycles       		= 0;										/* Bytes Send With No Data */
    sCommand.NbData            		= 1;										/* Bytes Send With Data */

	if (register_num == 1)
		sCommand.Instruction = W25Q_READ_SR1_CMD;
	else if (register_num == 2)
		sCommand.Instruction = W25Q_READ_SR2_CMD;
	else if (register_num == 3)
		sCommand.Instruction = W25Q_READ_SR3_CMD;
	else
		return HAL_ERROR;

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_OSPI_Receive(hospi, register_data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return HAL_ERROR;
    }

	return HAL_OK;
}

/* Write Status Registers Function */
HAL_StatusTypeDef W25Q128_Write_Status_Registers(OSPI_HandleTypeDef* hospi, uint8_t reg_data, uint8_t reg_num)
{
	OSPI_RegularCmdTypeDef sCommand;

	/* Common Commands*/
    sCommand.OperationType      	= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
    sCommand.FlashId            	= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
    sCommand.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
    sCommand.AddressDtrMode     	= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
    sCommand.DataDtrMode			= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
    sCommand.DQSMode            	= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
    sCommand.SIOOMode          		= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
    sCommand.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
    sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
    sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
    sCommand.AlternateBytesDtrMode	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
    sCommand.InstructionMode   		= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
    sCommand.InstructionSize    	= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
    sCommand.AddressSize 			= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
    /* Instruction */
    sCommand.Instruction 			= W25Q_WRITE_DISABLE_CMD;					/* What We Do? */
    /* Address */
    sCommand.AddressMode       		= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
    sCommand.Address				= 0;										/* Byte Address */
    /* Data */
    sCommand.DataMode          		= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
    sCommand.DummyCycles       		= 0;										/* Bytes Send With No Data */
    sCommand.NbData            		= 0;										/* Bytes Send With Data */

	if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK) {
        return HAL_ERROR;
    }

	sCommand.Instruction 			= W25Q_ENABLE_VOLATILE_SR_CMD;				/* What We Do? */
	if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK) {
        return HAL_ERROR;
    }

	if (reg_num == 1)
		sCommand.Instruction = W25Q_WRITE_SR1_CMD;
	else if (reg_num == 2)
		sCommand.Instruction = W25Q_WRITE_SR2_CMD;
	else if (reg_num == 3)
		sCommand.Instruction = W25Q_WRITE_SR3_CMD;
	else
		return HAL_ERROR;

	sCommand.DataMode          		= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData            		= 1;

	if (W25Q128_OSPI_WriteEnable(hospi) != HAL_OK) {
		return HAL_ERROR;
	}

	if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (HAL_OSPI_Transmit(hospi, &reg_data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK) {
        return HAL_ERROR;
    }

	return HAL_OK;
}
