/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "i2c.h"
int i2c_init(void) {
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */       
    I2C_BOARD_sda_SetDriveMode(CY_SYS_PINS_DM_RES_UP);
    I2C_BOARD_scl_SetDriveMode(CY_SYS_PINS_DM_RES_UP);
  
    I2C_BOARD_Start();
    
    return 0;
}

int i2c_read(uint8_t slave, uint8_t addr, uint8_t* data) {
    int result;
    result = I2C_BOARD_I2CMasterSendStart(slave,0x00);
    if(result) {
        I2C_BOARD_I2CMasterSendStop();
        return result;
    }
    result = I2C_BOARD_I2CMasterWriteByte(addr);
    if(result) {
        I2C_BOARD_I2CMasterSendStop();
        return result;
    }
    // Is de volgende regel fout?? (zie de 0x6A)
    //result = I2C_BOARD_I2CMasterSendRestart(0x6A,0x01);
    result = I2C_BOARD_I2CMasterSendRestart(slave,0x01);
    if(result) {
        I2C_BOARD_I2CMasterSendStop();
        return result;
    }
    //*data = I2C_BOARD_I2CMasterReadByte(I2C_BOARD_I2C_ACK_ADDR);
    *data = I2C_BOARD_I2CMasterReadByte(I2C_BOARD_I2C_NAK_DATA);
    I2C_BOARD_I2CMasterSendStop();
    return 0;
}
int i2c_write(uint8_t slave, uint8_t addr, uint8_t data) {
    int result;
    result = I2C_BOARD_I2CMasterSendStart(slave,0x00);
    if(result) {
        I2C_BOARD_I2CMasterSendStop();
        return result;
    }
    result = I2C_BOARD_I2CMasterWriteByte(addr);
    if(result) {
        I2C_BOARD_I2CMasterSendStop();
        return result;
    }
    result = I2C_BOARD_I2CMasterWriteByte(data);
    I2C_BOARD_I2CMasterSendStop();
    return result;
    
}
/* [] END OF FILE */
