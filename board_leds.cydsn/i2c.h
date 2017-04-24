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
#include <project.h>	

int i2c_init(void);
int i2c_read(uint8_t slave, uint8_t addr, uint8_t* data);
int i2c_write(uint8_t slave, uint8_t addr, uint8_t data);


/* [] END OF FILE */
