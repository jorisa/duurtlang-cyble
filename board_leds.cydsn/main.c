/* ========================================
 * Catan LED PSoC code
 *
 *
 *
 *
 * ========================================
*/
 
#include <main.h>
#include "i2c.h"
#include "bq2589x.h"
#include "tls2561.h"

#define TSL2561_ADDR  0x29

extern const uint32_t StripLights_CLUT[ ];

// Bitread, read a bit value from a byte
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

const uint8 gamma_corr[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

extern uint8_t updateLeds;


/* This flag is used by application to know whether a Central 
* device has been connected. This is updated in BLE event callback 
* function*/
extern uint8 deviceConnected;

/* 'restartAdvertisement' flag is used to restart advertisement */
extern uint8 restartAdvertisement;

// Global brightness is used by the app do decrease the brightness
uint8_t globalBrightness;



int main()
{
    int result;
    
    uint8_t data0;
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;
    uint8_t data4;
    uint8_t data5;
    
    i2c_init();
    
    // Light sensor:
    //result = i2c_write(TSL2561_ADDR, 0x00, 0x03);
    //result = i2c_read(TSL2561_ADDR, 0x00, &data2);
    
    //result = i2c_read(TSL2561_ADDR, 0x01, &data3); 
    //result = i2c_write(TSL2561_ADDR, 0x01, 0x03);
    //result = i2c_read(TSL2561_ADDR, 0x01, &data4);
   
    //bq2589x_init();
    
    /*
    result = i2c_read(0x6A, 0x04, &data0);
    result = i2c_read(0x6A, 0x07, &data1);
    result = i2c_read(0x6A, 0x0A, &data2);
    result = i2c_write(0x6A, 0x04, 0x10);  //1A
    result = i2c_write(0x6A, 0x07, 0x8D);  //Geen watchdog, rest default
    result = i2c_write(0x6A, 0x0A, 0x07);  //
    result = i2c_read(0x6A, 0x04, &data3);
    result = i2c_read(0x6A, 0x07, &data4);
    result = i2c_read(0x6A, 0x0A, &data5);
    */

       
    CyDelay(100);
    uint8_t lower1;
    uint8_t higher1;
    uint8_t lower2;
    uint8_t higher2;
    result = i2c_read(0x29, 0x0c, &lower1);
    result = i2c_read(0x29, 0x0d, &higher1);
    //result = i2c_read(0x29, 0x0e, &lower2);
    //result = i2c_read(0x29, 0x0f, &higher1);
    
    
    // Initialize StripLights
    StripLights_Start();  
	
	// Set dim level 0 = full power, 4 = lowest power
    StripLights_Dim(1); 
	
	// Clear all memory to black
	StripLights_MemClear(StripLights_BLACK);
    //StripLights_MemClear(StripLights_RED);
    //StripLights_MemClear(0x10);

	// Enable global interrupts, required for StripLights
    CyGlobalIntEnable;    

    // Start the BLE component and register StackEventHandler function
    CyBle_Start(StackEventHandler);    
    
    // Start the Real-time clock
    RTC_Start();  
    
    // Set the global brightness
    globalBrightness = 255;
    
    // Run the LED startup sequence
    while( StripLights_Ready() == 0);   
    StripLights_Trigger(1);  
    while( StripLights_Ready() == 0);   
    StripLights_Trigger(1);  
    
    // Turns boost mode on:
    //CH_OTG_Write(1);
    
    // Start the main loop
    for(;;) {
        
        // Process BLE stuff
        CyBle_ProcessEvents();
        
        if(deviceConnected == 1)
		{
			/* After the connection, send new connection parameter to the Client device 
			* to run the BLE communication on desired interval. This affects the data rate 
			* and power consumption. High connection interval will have lower data rate but 
			* lower power consumption. Low connection interval will have higher data rate at
			* expense of higher power. This function is called only once per connection. */
			UpdateConnectionParam();
		}
        
        if(restartAdvertisement)
		{
			/* Reset 'restartAdvertisement' flag*/
			restartAdvertisement = 0;

			/* Start Advertisement and enter Discoverable mode*/
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);	
		}
        
    // Read sensors

        
    // Determine mode
    
  
    // Determine output

        // Always update
        //updateLeds = 1;

        

        //Rainbow(30);

        uint8 resultCyb;
        
        //resultCyb = CyBle_GetState();
        //if (resultCyb == CYBLE_STATE_DISCONNECTED) {
        //    restartAdvertisement = 1; 
        //}
        
        
        // Update output
        if (updateLeds > 0) {
            updateLeds = 0;
            while( StripLights_Ready() == 0);   
            
            //uint16_t i;
            //for (i=0; i<2000; i++) {}
            
            // Als je de volgende regel toevoegt, gaat de eerste tile raar doen...
            //CyDelay(100);
            StripLights_Trigger(1);
        }
        //StripLights_Pixel(0,0,0x0000004F);
        //StripLights_Trigger(1);
        //CyDelay(200);
        //StripLights_Pixel(0,0,0x00000000);
        //StripLights_Trigger(1);
        //CyDelay(200);
        
        //bq2589x_get_status();
    }
}


        
            
    







//------------------------------------//
// LEDs for number, edge and water

void EdgeLED(uint8 brightness, uint16 offset,  uint32_t color)
{
    uint16_t i;
    for(i=9;i<20; i=i+2){
        StripLights_Pixel(i+offset, 0, corr_color(brightness, color));
    } 
}

void WaterLED(uint8 brightness, uint16 offset,  uint32_t color, uint8 tile)
{
    uint16 i;
    for(i=10;i<(20 + ExtraLED(tile)); i=i+2){
        StripLights_Pixel(i+offset, 0, corr_color(brightness, color));
    }
}


void allColor(uint16_t color)
{
    // Wait for last update to finish
    while( StripLights_Ready() == 0);  
    uint16_t i;
    for(i=0;i<400; i++){
        StripLights_Pixel(i, 0, color );
    }
    StripLights_Trigger(1);
}

// Clear a tile
void ClearLEDs(uint16 offset, uint8 LEDs) {
    uint16_t i;
    for(i=0;i<LEDs; i++){
        StripLights_Pixel(i + offset, 0, StripLights_BLACK );
    }
}

void ClearTile(uint8 tile) {
    uint16_t offset;
    uint16_t i;
    offset = offset_calc(tile);
    for(i=offset; i<offset+20+ExtraLED(tile); i++) {
        StripLights_Pixel(i, 0, StripLights_BLACK);
    }
}

// --------------------------
// Helper functions for number display

void NumberLED(uint8_t number, uint16_t offset, uint8 brightness, uint32_t color) {
    if (number >= 10) {
        StripLights_Pixel(7+offset, 0, corr_color(brightness, color));
        StripLights_Pixel(8+offset, 0, corr_color(brightness, color));
        number = number - 10;
    }
    switch (number) {
        case 0:
            //show_byte(63, offset, brightness, color);
        break;
        case 1:
            show_byte(6, offset, brightness, color);
        break;
        case 2:
            show_byte(91, offset, brightness, color);
        break;
        case 3:
            show_byte(79, offset, brightness, color);
        break;
        case 4:
            show_byte(102, offset, brightness, color);
        break;
        case 5:
            show_byte(109, offset, brightness, color);
        break;
        case 6:
            show_byte(125, offset, brightness, color);
        break;
        case 7:
            show_byte(7, offset, brightness, color);
        break;
        case 8:
            show_byte(127, offset, brightness, color);
        break;
        case 9:
            show_byte(111, offset, brightness, color);
        break;
    }
}

// Show a byte, used by the number
void show_byte(uint8_t val, uint16_t offset, uint8 brightness, uint32_t color) {
    uint16_t i;
    for(i=0;i<8;i++) {
        if (bitRead(val,i)==1) {
            StripLights_Pixel(i+offset, 0, corr_color(brightness, color));
            //StripLights_Pixel(i+offset, 0, color);
        }
    }
}

// Correct for led offsets
uint16_t offset_calc(uint8_t tile) {
    uint16_t offset = tile*20;
    switch (tile) {
        case 6:
            offset = offset + 1;
            break;
        case 7:
            offset = offset + 2;
            break;
        case 8:
            offset = offset + 3;
            break;
        case 9:
            offset = offset + 4;
            break;
        case 10:
            offset = offset + 5;
            break;
    }
    if (tile > 10)
    {
        offset = offset + 5;
    }
    return offset;
}

// Calculation for whether an extra LED is present
uint8_t ExtraLED(uint8_t tile) {
    switch (tile) {
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            return 1;
            break;
    }
    return 0;
}

// Returns the color that corrects for brightness
uint32_t corr_color(uint8 brightness, uint32_t color) {
    
    uint32_t corrected = 0;
    
    float b_corr = (globalBrightness/255.0)*(brightness/255.0);
    
    
    
    //corrected = (corrected) + (gamma_corr[(color >> 8) & 0xff] << 0);
    //corrected = (corrected) + (gamma_corr[(color >> 16) & 0xff] << 8);
    //corrected = (corrected) + (gamma_corr[(color >> 0) & 0xff] << 16);
    
    corrected = corrected + ((uint8)(b_corr*((color >> 0) & 0xff)) << 0);
    corrected = corrected + ((uint8)(b_corr*((color >> 8) & 0xff)) << 8);
    corrected = corrected + ((uint8)(b_corr*((color >> 16) & 0xff)) << 16);
     
    
    return corrected;
}

uint8_t tilePosCor(uint8_t tile) {
    if (tile == 11) { return 0;}
    if (tile == 17) { return 12;}
    if (tile == 18) { return 18;}
    return tile + 1;
}


/* [] END OF FILE */
