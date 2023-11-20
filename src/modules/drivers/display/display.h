//=====[#include guards - begin]===============================================

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define DISPLAY_IR_CLEAR_DISPLAY   0b00000001
#define DISPLAY_IR_ENTRY_MODE_SET  0b00000100
#define DISPLAY_IR_DISPLAY_CONTROL 0b00001000
#define DISPLAY_IR_FUNCTION_SET    0b00100000
#define DISPLAY_IR_SET_DDRAM_ADDR  0b10000000

#define DISPLAY_IR_ENTRY_MODE_SET_INCREMENT 0b00000010
#define DISPLAY_IR_ENTRY_MODE_SET_DECREMENT 0b00000000
#define DISPLAY_IR_ENTRY_MODE_SET_SHIFT     0b00000001
#define DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT  0b00000000

#define DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON  0b00000100
#define DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_OFF 0b00000000
#define DISPLAY_IR_DISPLAY_CONTROL_CURSOR_ON   0b00000010
#define DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF  0b00000000
#define DISPLAY_IR_DISPLAY_CONTROL_BLINK_ON    0b00000001
#define DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF   0b00000000

#define DISPLAY_IR_FUNCTION_SET_8BITS    0b00010000
#define DISPLAY_IR_FUNCTION_SET_4BITS    0b00000000
#define DISPLAY_IR_FUNCTION_SET_2LINES   0b00001000
#define DISPLAY_IR_FUNCTION_SET_1LINE    0b00000000
#define DISPLAY_IR_FUNCTION_SET_5x10DOTS 0b00000100
#define DISPLAY_IR_FUNCTION_SET_5x8DOTS  0b00000000

#define DISPLAY_20x4_LINE1_FIRST_CHARACTER_ADDRESS 0
#define DISPLAY_20x4_LINE2_FIRST_CHARACTER_ADDRESS 64
#define DISPLAY_20x4_LINE3_FIRST_CHARACTER_ADDRESS 20
#define DISPLAY_20x4_LINE4_FIRST_CHARACTER_ADDRESS 84

#define DISPLAY_RS_INSTRUCTION 0
#define DISPLAY_RS_DATA        1

#define DISPLAY_RW_WRITE 0
#define DISPLAY_RW_READ  1

#define DISPLAY_PIN_RS  4
#define DISPLAY_PIN_RW  5
#define DISPLAY_PIN_EN  6
#define DISPLAY_PIN_D0  7  
#define DISPLAY_PIN_D1  8  
#define DISPLAY_PIN_D2  9  
#define DISPLAY_PIN_D3 10
#define DISPLAY_PIN_D4 11
#define DISPLAY_PIN_D5 12 
#define DISPLAY_PIN_D6 13 
#define DISPLAY_PIN_D7 14 

#define DISPLAY_PIN_A_PCF8574 3

#define I2C1_SDA PB_9
#define I2C1_SCL PB_8

#define PCF8574_I2C_BUS_8BIT_WRITE_ADDRESS 78

#define SPI1_MOSI PA_7
#define SPI1_MISO PA_6
#define SPI1_SCK  PA_5
#define SPI1_CS   PD_14

#define DISPLAY_IR_SET_GDRAM_ADDR  0b10000000

#define DISPLAY_IR_FUNCTION_SET_EXTENDED_INSTRUCION_SET 0b00000100
#define DISPLAY_IR_FUNCTION_SET_BASIC_INSTRUCION_SET    0b00000000
#define DISPLAY_IR_FUNCTION_SET_GRAPHIC_DISPLAY_ON      0b00000010
#define DISPLAY_IR_FUNCTION_SET_GRAPHIC_DISPLAY_OFF     0b00000000

#define DISPLAY_ST7920_LINE1_FIRST_CHARACTER_ADDRESS 0
#define DISPLAY_ST7920_LINE2_FIRST_CHARACTER_ADDRESS 16
#define DISPLAY_ST7920_LINE3_FIRST_CHARACTER_ADDRESS 8
#define DISPLAY_ST7920_LINE4_FIRST_CHARACTER_ADDRESS 24

#define ST7920_SPI_SYNCHRONIZING_BIT_STRING 0b11111000

#define ST7920_SPI_RS_INSTRUCTION 0b000000000
#define ST7920_SPI_RS_DATA        0b000000010

#define ST7920_SPI_RW_WRITE 0b000000000
#define ST7920_SPI_RW_READ  0b000000100

//=====[Declaration of private data types]=====================================

typedef struct{
    int address;
    char data;
    bool displayPinRs;
    bool displayPinRw; 
    bool displayPinEn;
    bool displayPinA;
    bool displayPinD4;
    bool displayPinD5;
    bool displayPinD6;
    bool displayPinD7;
} pcf8574_t;

//=====[Declaration of public data types]======================================

typedef enum {
     DISPLAY_TYPE_LCD_HD44780, 
     DISPLAY_TYPE_GLCD_ST7920,
} displayType_t;

typedef enum {
     DISPLAY_CONNECTION_GPIO_4BITS,
     DISPLAY_CONNECTION_GPIO_8BITS,
     DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER,
     DISPLAY_CONNECTION_SPI
} displayConnection_t;

typedef enum {
     DISPLAY_MODE_CHAR, 
     DISPLAY_MODE_GRAPHIC
} displayMode_t;

typedef struct {
   displayConnection_t connection;
   displayType_t type;                                                                   
   displayMode_t mode;                                                  
} display_t;

//=====[Declarations (prototypes) of public functions]=========================

void displayInit( displayType_t type, displayConnection_t connection );
 
void displayCharPositionWrite( int charPositionX, int charPositionY );

void displayStringWrite( const char * str );

void displayClear( void );

// GLCD display only

void displayBitmapWrite( int* bitmap );

//=====[#include guards - end]=================================================

#endif // _DISPLAY_H_







// /*!****************************************************************************
//  * @file tds_sensor.h
//  * @brief TODO
//  * @author Quattrone Martin
//  * @date Oct 2023
//  *******************************************************************************/

// #ifndef DISPLAY_H
// #define DISPLAY_H

// namespace Drivers { 

//     class Display 
//     {
//         public:

//             //! Initialize the sensor
//             static void Init(int pin);

//             //! Returns the sensor object
//             static Display* GetInstance();

//             //!
//             void Read();

//         private:

//             Display(int pin) : mPin(pin) {}
//             ~Display() = default;
//             Display(const Display&) = delete;
//             Display& operator=(const Display&) = delete;

//             int mPin;
//             static Display* mInstance;
//     };

// } // namespace Drivers

// #endif // DISPLAY_H