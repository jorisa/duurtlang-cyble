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
#include "project.h"

int main(void)
{
    unsigned int address = 0;
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */       
    I2C_BOARD_sda_SetDriveMode(CY_SYS_PINS_DM_RES_UP);
    I2C_BOARD_scl_SetDriveMode(CY_SYS_PINS_DM_RES_UP);
  
    I2C_BOARD_Start();
    
    for(;;)
    {
        int result;
        int data;
        CyDelay(500);
        // Toggle on
        LED_DO_Write(1);
        // Toggle off
        
        //I2C_BOARD_I2CMasterSendStart(0x35,0x01);
        result = I2C_BOARD_I2CMasterSendStart(0x6A,0x00);
        if(result == 0) {
            I2C_BOARD_I2CMasterWriteByte(0x14);
            //data = I2C_BOARD_I2CMasterReadByte(I2C_BOARD_I2C_ACK_DATA);
            result = I2C_BOARD_I2CMasterSendRestart(0x6A,0x01);
            if(result==0) {
                data = I2C_BOARD_I2CMasterReadByte(I2C_BOARD_I2C_ACK_ADDR);
            }
        }
        I2C_BOARD_I2CMasterSendStop();
        address++;
        //result = I2C_BOARD_I2CMasterSendStart(0b0101001,0x01);
        //if(result == 0) {
        //    I2C_BOARD_I2CMasterSendStop();
        //}
        CyDelay(500);
        LED_DO_Write(0);
    }
}

/* [] END OF FILE */
