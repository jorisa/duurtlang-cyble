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

/********************************************************************************
*	Contains all macros and function declaration used in the main.c file 
********************************************************************************/
#include <project.h>
#include <BLEApplications.h>
#include <demo.h>


// Funtion definitions
void NumberLED(uint8 ledPosition, uint16 offset, uint8 brightness, uint32_t color);
void EdgeLED(uint8 brightness, uint16 offset, uint32_t color);
void WaterLED(uint8 brightness, uint16 offset,  uint32_t color, uint8 tile);

void allColor(uint16_t color);
void ClearLEDs(uint16 offset, uint8 LEDs);
void ClearTile(uint8 tile);

void show_byte(uint8_t val, uint16_t offset, uint8 brightness, uint32_t color);

uint16 offset_calc(uint8 tile);
uint8 ExtraLED(uint8 tile);

uint32_t corr_color(uint8 brightness, uint32_t color);

uint8_t tilePosCor(uint8_t tile);

// Variable definitions



/* [] END OF FILE */
