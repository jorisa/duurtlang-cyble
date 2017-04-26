
// Luminosity sensor, TSL2561
// Address b7=0,b6=1,b5=1,b4=1,b3=0,b2=0,b1=1, b0=R/W
#define TSL2561_ADDRESS_GND         (0x29 << 1)
#define TSL2561_ADDRESS_FLOAT       (0x39 << 1)
#define TSL2561_ADDRESS_VDD         (0x49 << 1)
 
////////////// Registers //////////////////////////////////
// Register definition
#define TSL2561_CONTROL             0x00
#define TSL2561_TIMING              0x01
#define TSL2561_THRESHLOWLOW        0x02
#define TSL2561_THRESHHIGHLOW       0x04
#define TSL2561_INTERRUPT           0x06
#define TSL2561_CRC                 0x08
#define TSL2561_ID                  0x0A
#define TSL2561_DATA0LOW            0x0C
#define TSL2561_DATA0HIGH           0x0D
#define TSL2561_DATA1LOW            0x0E
#define TSL2561_DATA1HIGH           0x0F
 
////////////// TIMING PARAMETER ///////////////////////////
#define TIMING_GAIN_1               (0UL << 4)
#define TIMING_GAIN_16              (1UL << 4)
#define TIMING_TIME_13R7            (0x0)
#define TIMING_TIME_101             (0x1)
#define TIMING_TIME_402             (0x2)
#define TIMING_TIME_MANU            (0x3)
#define TIMING_DEFAULT              (TIMING_GAIN_1 + TIMING_TIME_402)
 
////////////// ID /////////////////////////////////////////
#define I_AM_TSL2561                0x50
#define REG_NO_MASK                 0x0F
 
////////////// COMMAND ////////////////////////////////////
#define CMD_CMDMODE                 (1UL << 7)
#define CMD_CLEAR                   (1UL << 6)
#define CMD_WORD                    (1UL << 5)
#define CMD_BLOCK                   (1UL << 4)
#define CMD_SINGLE                  (CMD_CMDMODE)
#define CMD_MULTI                   (CMD_CMDMODE + CMD_WORD)