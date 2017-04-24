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

#include <main.h>

extern const uint32_t StripLights_CLUT[ ];


/*
void fadeTile(){
    uint8 j;
    
    for(j=0;j<4; j++) {
        uint16 i;
        for(i=250; i>0; i=i-5) {
            StripLights_Pixel(offset_calc(18)+15, 0, i);
            StripLights_Pixel(offset_calc(18)+13, 0, i);
            StripLights_Pixel(offset_calc(18)+11, 0, i);
            StripLights_Pixel(offset_calc(18)+17, 0, i);
            StripLights_Pixel(offset_calc(18)+19, 0, i);
            StripLights_Pixel(offset_calc(18)+9, 0, i);
            StripLights_Trigger(1);
            CyDelay(20);
        }
        for(i=0; i<250; i=i+5) {
            StripLights_Pixel(offset_calc(18)+15, 0, i);
            StripLights_Pixel(offset_calc(18)+13, 0, i);
            StripLights_Pixel(offset_calc(18)+11, 0, i);
            StripLights_Pixel(offset_calc(18)+17, 0, i);
            StripLights_Pixel(offset_calc(18)+19, 0, i);
            StripLights_Pixel(offset_calc(18)+9, 0, i);
            StripLights_Trigger(1);
            CyDelay(20);
        }

    }   
}

void diceChange(){
    while( StripLights_Ready() == 0);
    ClearTile(18);
    uint8 number = diceRand%12;
    diceRand = diceRand + 5;
    NumberLED(number, offset_calc(18), 0xFF, 0x00FFF000);
    StripLights_Trigger(1);
    CyDelay(30);
}


*/




/************************************************
 *                    Clock()
 *
 *  Starts a clock function.
 *
 ************************************************/

/*
void Clock()
{
    // Get the current time
    // Show the seconds in the middle tile
    uint8_t second;
    second = RTC_GetSecond(RTC_GetTime());
    uint8_t minute;
    minute = RTC_GetMinutes(RTC_GetTime());
    //ClearLEDs(offset_calc(17),21);
    //ClearLEDs(offset_calc(18),21);
    
    StripLights_MemClear(StripLights_BLACK);
    NumberLED(second/10,offset_calc(17),140,21845);
    NumberLED(second%10,offset_calc(18),140,21845);

    NumberLED(minute/10,offset_calc(16),140,65280);
    NumberLED(minute%10,offset_calc(15),140,65280);
    
    uint8_t i;
    
    for(i=1; i<13; i++){
        edge(offset_calc(-(i+5)%12+11),140,16711680);
        number(i,offset_calc(-(i+5)%12+11),140,255);
    }
    
    StripLights_Trigger(1); 
    
    //RTC_Update();
    CyDelay(10);
    
    
}
*/


/************************************************
 *                    SingleLED()
 *
 *  Rotate a single LED around the ring.
 *
 ************************************************/
/*
void SingleLED(uint32_t delay)
{
    uint32_t ledPosition = 0;  // On LED position

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

*/


/************************************************
 *                    SingleLED()
 *
 *  Rotate a single LED around the ring.
 *
 ************************************************/

/*
void SingleLedMultiColor(uint32_t delay)
{
    uint32_t ledPosition = 0;  // On LED position
	uint32_t colorIndex = 0;

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
*/


/************************************************
 *                   OppositeRings()
 *
 *  A green and red LEDs go around the ring in 
 *  opposite directions, with tails.
 *
 ************************************************/

/*
void OppositeRings(uint32_t delay)
{
    uint32_t fwdLED = 0;  // Forward LED Position
	uint32_t bwdLED = 0;  // Backward LED Postion

	uint32_t oldColor;    // Temp Color
	uint32_t loop;        // Loop counter
	
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

*/
/************************************************
 *                    RgbChase()
 *
 *  Rotate three LED colors around the ring.
 *  Make tails by dimming the LEDs each time.
 *
 ************************************************/

/*
void RgbChase(uint32_t delay)
{
    uint32_t redPosition   = 0; // Initial color LED positions
	uint32_t greenPosition = StripLights_TOTAL_LEDS/3;
	uint32_t bluePosition  = (StripLights_TOTAL_LEDS * 2)/3;
	
	uint32_t oldColor;   // Temp color value
	uint32_t loop;       // Loop counter
	
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
	
        if(RTC_GetSecond(RTC_GetTime())>6) break;
        
        // If SW1 pressed, leave loop
	    if(SW1_Read() == 0) break;
    }	
	// Wait for SW1 to be released then delay a bit for debouncing
	while(SW1_Read() == 0);
	CyDelay(50);
}
    
*/

/************************************************
 *                    Rainbow()
 *
 *  Use the colorwheel section of the color lookup
 *  table to create a rotating rainbow.
 *
 ************************************************/


void Rainbow(uint32_t delay)
{
    uint32_t startColor = 0;  // Index color in colorwheel
	uint32_t ledPosition = 0; // LED position when setting color         
    uint32_t color = 0;       // Temp color to set LED
	
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

/*
void OneColor(uint32_t delay)
{
    uint32_t color = 0;  // Index color in colorwheel
	uint32_t nextColor = 0;
	uint32_t pct = 0;
	uint32_t toColor, fromColor, newColor;
	uint32_t ledPosition = 0; // LED position when setting color         
	
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

*/


/* [] END OF FILE */
