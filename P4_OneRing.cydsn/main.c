/* ========================================
 *
 *  This project was written to have a little
 *  fun with the NeoPixel LED rings that are 
 *  sold by Adafruit.com.  They use the WS2812
 *  RGB LED modules that allow you to string
 *  a thousand or more RGB LEDs together on a
 *  single IO pin.
 *
 *  The StripLight component makes using these
 *  module very easy, so you can use your time
 *  to code fun light patterns instead of worrying
 *  about tight timing loops, so enjoy.
 *
 *  To use, make sure you have the "Din" pin to the
 *  GPIO pin that you have connected your LED ring
 *  to.  Also set the "LEDs_per_strip" parameter to
 *  the number of LEDs in your ring, then build the
 *  project and reprogram your PSoC.
 *
 *  Press the switch to advance to the next pattern.
 *
 *  Pin Din  P02  Serial data to WS2812 module/s
 *      SW1  P07  Mode Switch.
 *
 *  4-2-2014   1.0  Mark Hastings  First code
 *  4-4-2014   1.1  Mark Hastings  Some minor changes
 * 10-5-2014   1.2  Mark Hastings  Added some fun patterns for the LED rings.
 *
 * ========================================
*/
 
#include <project.h>

// Functions for BLE handling
void StackEventHandler(uint32 event, void* eventParam);

// Function prototypes for pattern functions.
void SingleLED(uint32 delay);
void RgbChase(uint32 delay);
void Clock(uint32 delay);
void Rainbow(uint32 delay);
void OppositeRings(uint32 delay);
void OneColor(uint32 delay);
void SingleLedMultiColor(uint32 delay);
void SingleLEDRand();
void NumberLED(uint8 ledPosition, uint16 offset, uint8 brightness, uint32 color);
void EdgeLED(uint8 brightness, uint16 offset, uint32 color);
void WaterLED(uint8 brightness, uint16 offset,  uint32 color);

void number(uint8_t number, uint16_t offset, uint8 brightness, uint32_t color);
void show_byte(uint8_t val, uint16_t offset, uint8 brightness, uint32_t color);

void ClearLEDs(uint16 offset, uint8 LEDs);

void edge(uint16_t offset, uint8 brightness, uint32 color);
void water(uint16_t offset, uint8 brightness, uint32 color);

void allColor(uint16_t color);

uint32 corr_color(uint8 brightness, uint32 color);

extern const uint32 StripLights_CLUT[ ];

// Bitread, copied from photon, read a bit value from a byte
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

uint8 gamma_corr[] = {
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


int main()
{
    // Initialize StripLights
    StripLights_Start();  
	
	// Set dim level 0 = full power, 4 = lowest power
    StripLights_Dim(1); 
	
	// Clear all memory to black
	StripLights_MemClear(StripLights_BLACK);

	// Enable global interrupts, required for StripLights
    CyGlobalIntEnable;    

    /* Start the BLE component and register StackEventHandler function */
    CyBle_Start(StackEventHandler);    
    
	// Loop through the different patterns each time SW1 is pressed.
    
    uint16_t colorVal = 0;
    
	for(;;)
	{
        CyBle_ProcessEvents();
        //colorVal++;
        //allColor(colorVal);
        //CyDelay(1);
        /*
        uint8_t i;
        for(i=0;i<=12;i++) {
        StripLights_MemClear(StripLights_BLACK);
        number(i,1);
        StripLights_Trigger(1);
        CyDelay(1000);
        */
        
        //SingleLEDRand();
        //SingleLED2(50);
        
        /*
		SingleLedMultiColor(100);
		OneColor(20);
		SingleLED(50);
		OppositeRings(130);
		RgbChase(100);
		Rainbow(30);
        */
        
	}
}


/*******************************************************************************
* Function Name: StackEventHandler
********************************************************************************
*
* Summary:
*  This is an event callback function to receive events from the BLE Component.
*
* Parameters:  
*  uint8 event:       Event from the CYBLE component
*  void* eventParams: A structure instance for corresponding event type. The 
*                     list of event structure is described in the component 
*                     datasheet.
*
* Return: 
*  None
*
*******************************************************************************/
void StackEventHandler(uint32 event, void *eventParam)
{
    
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
    
    switch(event)
    {
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            break;
        case CYBLE_EVT_GATTS_WRITE_REQ:
        case CYBLE_EVT_GATTS_WRITE_CMD_REQ:
            
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam; //read in event command
            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_CATAN_TILE_CHAR_HANDLE) //If client writes to the number_write characteristic
                {
                    uint16 tile = wrReqParam->handleValPair.value.val[0];
                    
                    uint8 number_write = wrReqParam->handleValPair.value.val[1]; //Pull out the number_write value
                    
                    uint8 bright_num = wrReqParam->handleValPair.value.val[2];
                   
                    uint32 color_num = 0;
                    color_num = wrReqParam->handleValPair.value.val[3];
                    color_num = (color_num << 8) + wrReqParam->handleValPair.value.val[4];
                    color_num = (color_num << 8) + wrReqParam->handleValPair.value.val[5];

                    uint8 bright_edge = wrReqParam->handleValPair.value.val[6];
                   
                    uint32 color_edge = 0;
                    color_edge = wrReqParam->handleValPair.value.val[7];
                    color_edge = (color_edge << 8) + wrReqParam->handleValPair.value.val[8];
                    color_edge = (color_edge << 8) + wrReqParam->handleValPair.value.val[9];
                    
                    uint8 bright_water = wrReqParam->handleValPair.value.val[10];
                    
                    uint32 color_water = 0;
                    color_water = wrReqParam->handleValPair.value.val[11];
                    color_water = (color_water << 8) + wrReqParam->handleValPair.value.val[12];
                    color_water = (color_water << 8) + wrReqParam->handleValPair.value.val[13];                

                    CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED); 
                    
                    // Clear specific tile
                    ClearLEDs(tile*20,21);
                    
                    NumberLED(number_write, tile*20, bright_num, color_num);
                    EdgeLED(bright_edge, tile*20,  color_edge);
                    WaterLED(bright_water, tile*20,  color_water);
                }   
 
            if (event == CYBLE_EVT_GATTS_WRITE_REQ) //If write was a write with response request.
                {
                    CyBle_GattsWriteRsp(cyBle_connHandle); //Send response back to Client
                }
            

            
            break;
            
            
        default:
    	    break;
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

// Display a number

void number(uint8_t number, uint16_t offset, uint8 brightness, uint32_t color) {
    if (number >= 10) {
        StripLights_Pixel(7+offset, 0, corr_color(brightness, color));
        StripLights_Pixel(8+offset, 0, corr_color(brightness, color));
        number = number - 10;
    }
    switch (number) {
        case 0:
            show_byte(63, offset, brightness, color);
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

void edge(uint16 offset, uint8 brightness, uint32 color) {
    uint16_t i;
    for(i=9;i<20; i=i+2){
        StripLights_Pixel(i+offset, 0, corr_color(brightness, color));
    }
}

void water(uint16 offset, uint8 brightness, uint32 color) {
    uint16 i;
    for(i=10;i<21; i=i+2){
        StripLights_Pixel(i+offset, 0, corr_color(brightness, color));
    }
}

uint32 corr_color(uint8 brightness, uint32 color) {
    
    uint32 corrected = 0;
    
    float b_corr = brightness/255.0;
    
    //corrected = (corrected) + (gamma_corr[(color >> 8) & 0xff] << 0);
    //corrected = (corrected) + (gamma_corr[(color >> 16) & 0xff] << 8);
    //corrected = (corrected) + (gamma_corr[(color >> 0) & 0xff] << 16);
    
    corrected = corrected + ((uint8)(b_corr*((color >> 0) & 0xff)) << 0);
    corrected = corrected + ((uint8)(b_corr*((color >> 8) & 0xff)) << 8);
    corrected = corrected + ((uint8)(b_corr*((color >> 16) & 0xff)) << 16);
     
    
    return corrected;
}




/************************************************
 *                    SingleLED()
 *
 *  Rotate a single LED around the ring.
 *
 ************************************************/

void SingleLEDRand()
{
    uint32 ledPosition = 3;

		// Wait for last update to finish
    while( StripLights_Ready() == 0);                 
	
	// Clear all LEDs to background color
	//StripLights_MemClear(StripLights_BLACK);
    StripLights_MemClear(0x000A0000);
	
	// Set the color of a single LED
    //StripLights_Pixel(ledPosition, 0, StripLights_RED ); 
    //StripLights_Pixel(ledPosition, 0, 0x0000000F );

	// Trigger update of all LEDs at once
    StripLights_Trigger(1);

    CyDelay(200);
    
    	// Set the color of a single LED
    //StripLights_Pixel(ledPosition, 0, StripLights_BLUE );
    StripLights_Pixel(ledPosition, 0, 0x000000AF ); 

	// Trigger update of all LEDs at once
    StripLights_Trigger(1);
        
    CyDelay(200);
}

void NumberLED(uint8 ledPosition, uint16 offset, uint8 brightness,  uint32 color)
{
    // Wait for last update to finish
    while( StripLights_Ready() == 0);                 
	
	// Clear all LEDs to background color
	//StripLights_MemClear(StripLights_BLACK);   
	
	
    number(ledPosition,offset, brightness, color);

	// Trigger update of all LEDs at once
    //StripLights_Trigger(1);   
}

void EdgeLED(uint8 brightness, uint16 offset,  uint32 color)
{
    // Wait for last update to finish
    while( StripLights_Ready() == 0);                 
	
	// Clear all LEDs to background color
	//StripLights_MemClear(StripLights_BLACK);   
	
	
    edge(offset, brightness, color);

	// Trigger update of all LEDs at once
    //StripLights_Trigger(1);   
}

void WaterLED(uint8 brightness, uint16 offset,  uint32 color)
{
    // Wait for last update to finish
    while( StripLights_Ready() == 0);                 
	
	// Clear all LEDs to background color
	//StripLights_MemClear(StripLights_BLACK);   
	
	
    water(offset, brightness, color);

	// Trigger update of all LEDs at once
    StripLights_Trigger(1);   
}
    
/************************************************
 *                    SingleLED()
 *
 *  Rotate a single LED around the ring.
 *
 ************************************************/

void SingleLED(uint32 delay)
{
    uint32 ledPosition = 0;  // On LED position

	// Loop until SW1 pressed
    for(;;)  
    {
		// Wait for last update to finish
	    while( StripLights_Ready() == 0);                 
		
		// Clear all LEDs to background color
		StripLights_MemClear(StripLights_BLACK);   
		
		// Set the color of a single LED
	    StripLights_Pixel(ledPosition, 0, StripLights_RED ); 

		// Trigger update of all LEDs at once
        StripLights_Trigger(1);    
		
		// Loop delay
        CyDelay(delay);    

		// Advance to next position.  If too far, start back at 0
		ledPosition++;  
		if(ledPosition >= StripLights_TOTAL_LEDS) ledPosition = 0;

		// If SW1 is pressed, leave loop
		if(SW1_Read() == 0) break; 
    }	
	
	// Wait for button release and delay to debounce switch
	while(SW1_Read() == 0);   
	CyDelay(50);
}




/************************************************
 *                    SingleLED()
 *
 *  Rotate a single LED around the ring.
 *
 ************************************************/
void SingleLedMultiColor(uint32 delay)
{
    uint32 ledPosition = 0;  // On LED position
	uint32 colorIndex = 0;

	// Loop until SW1 pressed
    for(;;)  
    {
		
		// Wait for last update to finish
	    while( StripLights_Ready() == 0);                 
		
		// Clear all LEDs to background color
		StripLights_MemClear(StripLights_BLACK);   
		
		// Set the color of a single LED
	    StripLights_Pixel(ledPosition, 0,  StripLights_getColor( colorIndex ) ); 

		// Trigger update of all LEDs at once
        StripLights_Trigger(1);    
		
		// Loop delay
        CyDelay(delay);    

		// Advance to next position.  If too far, start back at 0
		ledPosition++;  
		if(ledPosition >= StripLights_TOTAL_LEDS) ledPosition = 0;
		
		colorIndex++;  
        if(colorIndex >= StripLights_COLOR_WHEEL_SIZE) colorIndex = 0;

		// If SW1 is pressed, leave loop
		if(SW1_Read() == 0) break; 
    }	
	
	// Wait for button release and delay to debounce switch
	while(SW1_Read() == 0);   
	CyDelay(50);
}



/************************************************
 *                   OppositeRings()
 *
 *  A green and red LEDs go around the ring in 
 *  opposite directions, with tails.
 *
 ************************************************/
void OppositeRings(uint32 delay)
{
    uint32 fwdLED = 0;  // Forward LED Position
	uint32 bwdLED = 0;  // Backward LED Postion

	uint32 oldColor;    // Temp Color
	uint32 loop;        // Loop counter
	
	// Loop until button pressed
	for(;;)
	{
		// Loop through one revolution
        for(fwdLED = 0; fwdLED < StripLights_TOTAL_LEDS; fwdLED++)
	    {
			// Calculate backward LED position
			bwdLED = StripLights_TOTAL_LEDS - fwdLED;
			
			// Wait for last update to complete
			while( StripLights_Ready() == 0);
			
			// Loop around and dim all the LEDs to create tails
			for(loop = 0; loop < StripLights_TOTAL_LEDS; loop++)
			{
				oldColor = StripLights_GetPixel(loop, 0);
				oldColor = (oldColor >> 1) & 0x007F7F7F;
				StripLights_Pixel(loop, 0, oldColor );	
			}

			// Blend the forward LED with existing colors
			oldColor = StripLights_GetPixel(fwdLED, 0) | StripLights_GREEN;
		    StripLights_Pixel(fwdLED, 0, oldColor );
			
			// Blend the backward LED with existing colors
			oldColor = StripLights_GetPixel(bwdLED, 0) | StripLights_RED;
		    StripLights_Pixel(bwdLED, 0,  oldColor );
			
			// Udate all the LEDs at once
	        StripLights_Trigger(1);
			
			// Loop delay
			CyDelay(delay);
		
			// Leave loop is SW1 pressed
		    if(SW1_Read() == 0) break;
	    }

		// Leave if SW1 pressed
	    if(SW1_Read() == 0) break;
    }
	
	// Wait for SW1 to be released then delay a bit for debouncing
	while(SW1_Read() == 0);
	CyDelay(50);
}

/************************************************
 *                    RgbChase()
 *
 *  Rotate three LED colors around the ring.
 *  Make tails by dimming the LEDs each time.
 *
 ************************************************/
void RgbChase(uint32 delay)
{
    uint32 redPosition   = 0; // Initial color LED positions
	uint32 greenPosition = StripLights_TOTAL_LEDS/3;
	uint32 bluePosition  = (StripLights_TOTAL_LEDS * 2)/3;
	
	uint32 oldColor;   // Temp color value
	uint32 loop;       // Loop counter
	
	// Loop until SW1 pressed
    for(;;)
    {
		// Wait for last update to complete
		while( StripLights_Ready() == 0);
		
		// Loop through and dim all LEDs by half
		for(loop = 0; loop < StripLights_TOTAL_LEDS; loop++)
		{
			oldColor = StripLights_GetPixel(loop, 0);
			oldColor = (oldColor >> 1) & 0x007F7F7F;
			StripLights_Pixel(loop, 0, oldColor );	
		}
		
		// Set the three LEDs at their new position
	    StripLights_Pixel(redPosition,   0,  StripLights_RED );
	    StripLights_Pixel(greenPosition, 0,  StripLights_GREEN );
		StripLights_Pixel(bluePosition,  0,  StripLights_BLUE );
		
		// Trigger to update all the LEDs at once
        StripLights_Trigger(1);
		
		// Loop delay in mSeconds
        CyDelay(delay);
		
		// Calculate new LED positions around the ring
	    redPosition   = (redPosition + 1)   % StripLights_TOTAL_LEDS;
	    greenPosition = (greenPosition + 1) % StripLights_TOTAL_LEDS;
	    bluePosition  = (bluePosition + 1)  % StripLights_TOTAL_LEDS;
	
        // If SW1 pressed, leave loop
	    if(SW1_Read() == 0) break;
    }	
	// Wait for SW1 to be released then delay a bit for debouncing
	while(SW1_Read() == 0);
	CyDelay(50);
}

/************************************************
 *                    Rainbow()
 *
 *  Use the colorwheel section of the color lookup
 *  table to create a rotating rainbow.
 *
 ************************************************/
void Rainbow(uint32 delay)
{
    uint32 startColor = 0;  // Index color in colorwheel
	uint32 ledPosition = 0; // LED position when setting color         
    uint32 color = 0;       // Temp color to set LED
	
    for(;;)
    {   
		// Wait for last update to complete
        while( StripLights_Ready() == 0);  
		
		// Set new start color
        color = startColor;    
		
		// Loop through all LEDs giving each one a different color from the color wheel
        for(ledPosition = 0; ledPosition <= StripLights_TOTAL_LEDS; ledPosition++)
        {
            StripLights_Pixel(ledPosition, 0, StripLights_getColor( color ));
            color++;
            if(color >= StripLights_COLOR_WHEEL_SIZE) color = 0;
        }
		
		// Update the LEDs all at once
		StripLights_Trigger(1); 
		
        // Each time through the main loop start with different color
		// Wrap around at end of color wheel
		startColor++;  
        if(startColor >= StripLights_COLOR_WHEEL_SIZE) startColor = 0;
    
        CyDelay(delay);   // Loop delay

	    if(SW1_Read() == 0) break;  // If SW1 pressed leave
    }	
	// Wait for SW1 to be released then delay a bit for debouncing
	while(SW1_Read() == 0);         
	CyDelay(50);
}
/************************************************
 *                    OneColor()
 *
 *  Use the colorwheel section of the color lookup
 *  table to write all LEDs the same color.
 *
 ************************************************/
void OneColor(uint32 delay)
{
    uint32 color = 0;  // Index color in colorwheel
	uint32 nextColor = 0;
	uint32 pct = 0;
	uint32 toColor, fromColor, newColor;
	uint32 ledPosition = 0; // LED position when setting color         
	
    for(;;)
    {   
		nextColor = color + 1;
		if(nextColor >= StripLights_COLOR_WHEEL_SIZE) nextColor = 0;
		
		fromColor = StripLights_CLUT[color];
		toColor   = StripLights_CLUT[nextColor];  

		for(pct = 0; pct <= 100; pct += 10)
		{
		    newColor  = (((pct * (toColor & 0x00FF0000)) + ((100-pct) * (fromColor & 0x00FF0000)))/100) & 0x00FF0000;
            newColor |= (((pct * (toColor & 0x0000FF00)) + ((100-pct) * (fromColor & 0x0000FF00)))/100) & 0x0000FF00;
            newColor |= (((pct * (toColor & 0x000000FF)) + ((100-pct) * (fromColor & 0x000000FF)))/100) & 0x000000FF;	
			
			// Wait for last update to complete
			 while( StripLights_Ready() == 0); 
			
			// Loop through all LEDs giving each one a different color from the color wheel
	        for(ledPosition = 0; ledPosition <= StripLights_TOTAL_LEDS; ledPosition++)
	        {
	            StripLights_Pixel(ledPosition, 0, newColor);
	        }
 		
		    // Update the LEDs all at once
	  	    StripLights_Trigger(1); 
			
			CyDelay(delay);   // Loop delay
	    } 
        // Each time through the main loop start with different color
		// Wrap around at end of color wheel
		color++;  
        if(color >= StripLights_COLOR_WHEEL_SIZE) color = 0;

	    if(SW1_Read() == 0) break;  // If SW1 pressed leave
    }	
	// Wait for SW1 to be released then delay a bit for debouncing
	while(SW1_Read() == 0);         
	CyDelay(50);
}



/* [] END OF FILE */
