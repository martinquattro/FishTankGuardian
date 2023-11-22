//=====[Libraries]=============================================================

#include "display.h"

#include "arm_book_lib.h"
#include "mbed.h"

//=====[Declaration and initialization of public global objects]===============

DigitalOut displayD0( D0 );
DigitalOut displayD1( D1 );
DigitalOut displayD2( D2 );
DigitalOut displayD3( D3 );
DigitalOut displayD4( D4 );
DigitalOut displayD5( D5 );
DigitalOut displayD6( D6 );
DigitalOut displayD7( D7 );
DigitalOut displayRs( D8 );
DigitalOut displayEn( D9 );

I2C i2cPcf8574( I2C1_SDA, I2C1_SCL ); 

DigitalOut spiSt7920ChipSelect(SPI1_CS);
SPI spiSt7920(SPI1_MOSI, SPI1_MISO, SPI1_SCK);

//=====[Declaration and initialization of private global variables]============

static display_t display;
static pcf8574_t pcf8574;
static bool initial8BitCommunicationIsCompleted;

//=====[Declarations (prototypes) of private functions]========================

static void displayPinWrite( int pinName, int value );
static void displayDataBusWrite( int dataByte );
static void displayCodeWrite( bool type, int dataBus );

//=====[Implementations of public functions]===================================

//-----------------------------------------------------------------------------
void displayInit(displayType_t type, displayConnection_t connection)
{   
    display.type = type;
    display.connection = connection;
    display.mode = DISPLAY_MODE_CHAR;
    
    initial8BitCommunicationIsCompleted = false;    

    delay( 50 );
    
    displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                      DISPLAY_IR_FUNCTION_SET | 
                      DISPLAY_IR_FUNCTION_SET_8BITS );
    delay( 5 );
            
    displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                      DISPLAY_IR_FUNCTION_SET | 
                      DISPLAY_IR_FUNCTION_SET_8BITS );
    delay( 1 ); 

    displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                      DISPLAY_IR_FUNCTION_SET | 
                      DISPLAY_IR_FUNCTION_SET_8BITS );
    delay( 1 );  

    switch( display.connection ) {
        case DISPLAY_CONNECTION_GPIO_8BITS:
        case DISPLAY_CONNECTION_SPI:
            displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                              DISPLAY_IR_FUNCTION_SET | 
                              DISPLAY_IR_FUNCTION_SET_8BITS | 
                              DISPLAY_IR_FUNCTION_SET_2LINES |
                              DISPLAY_IR_FUNCTION_SET_5x8DOTS );
            delay( 1 );         
        break;
        
        case DISPLAY_CONNECTION_GPIO_4BITS:
        case DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER:
            displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                              DISPLAY_IR_FUNCTION_SET | 
                              DISPLAY_IR_FUNCTION_SET_4BITS );
            delay( 1 );  

            initial8BitCommunicationIsCompleted = true;  

            displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                              DISPLAY_IR_FUNCTION_SET | 
                              DISPLAY_IR_FUNCTION_SET_4BITS | 
                              DISPLAY_IR_FUNCTION_SET_2LINES |
                              DISPLAY_IR_FUNCTION_SET_5x8DOTS );
            delay( 1 );                                      
        break;
    }

    displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                      DISPLAY_IR_DISPLAY_CONTROL |
                      DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_OFF |      
                      DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF |       
                      DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF );       
    delay( 1 );          

    displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                      DISPLAY_IR_CLEAR_DISPLAY );       
    delay( 1 ); 

    displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                      DISPLAY_IR_ENTRY_MODE_SET |
                      DISPLAY_IR_ENTRY_MODE_SET_INCREMENT |       
                      DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT );                  
    delay( 1 );           

    displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                      DISPLAY_IR_DISPLAY_CONTROL |
                      DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON |      
                      DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF |    
                      DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF );    
    delay( 1 );  
}

//-----------------------------------------------------------------------------
void displayCharPositionWrite( int charPositionX, int charPositionY )
{    
    if( display.type == DISPLAY_TYPE_LCD_HD44780 )
    {
        switch( charPositionY ) {
            case 0:
                displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                                  DISPLAY_IR_SET_DDRAM_ADDR |
                                  ( DISPLAY_20x4_LINE1_FIRST_CHARACTER_ADDRESS +
                                    charPositionX ) );
                delay( 1 );         
            break;
        
            case 1:
                displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                                  DISPLAY_IR_SET_DDRAM_ADDR |
                                  ( DISPLAY_20x4_LINE2_FIRST_CHARACTER_ADDRESS +
                                    charPositionX ) );
                delay( 1 );         
            break;
        
            case 2:
                displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                                  DISPLAY_IR_SET_DDRAM_ADDR |
                                  ( DISPLAY_20x4_LINE3_FIRST_CHARACTER_ADDRESS +
                                    charPositionX ) );
                delay( 1 );         
            break;

            case 3:
                displayCodeWrite( DISPLAY_RS_INSTRUCTION, 
                                  DISPLAY_IR_SET_DDRAM_ADDR |
                                  ( DISPLAY_20x4_LINE4_FIRST_CHARACTER_ADDRESS +
                                    charPositionX ) );
                delay( 1 );         
            break;
         }
    }
}

//-----------------------------------------------------------------------------
void displayStringWrite( const char * str )
{
    while (*str) 
    {
        displayCodeWrite(DISPLAY_RS_DATA, *str++);
    }
}

//-----------------------------------------------------------------------------
void displayClear( void )
{
    displayCodeWrite(DISPLAY_RS_INSTRUCTION, DISPLAY_IR_CLEAR_DISPLAY);
    delay(2); 
}

//=====[Implementations of private functions]==================================

//----static-------------------------------------------------------------------
static void displayCodeWrite(bool type, int dataBus)
{
    switch(display.connection) 
    {
        case DISPLAY_CONNECTION_GPIO_8BITS:
        case DISPLAY_CONNECTION_GPIO_4BITS:
        case DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER:    
            if (type == DISPLAY_RS_INSTRUCTION)
                displayPinWrite( DISPLAY_PIN_RS, DISPLAY_RS_INSTRUCTION);
                else
                displayPinWrite( DISPLAY_PIN_RS, DISPLAY_RS_DATA);
            displayPinWrite( DISPLAY_PIN_RW, DISPLAY_RW_WRITE );
            displayDataBusWrite( dataBus );
        break;
  
        case DISPLAY_CONNECTION_SPI:
            spiSt7920.lock();
            spiSt7920ChipSelect = ON;
            if ( type == DISPLAY_RS_INSTRUCTION )           
                spiSt7920.write( ST7920_SPI_SYNCHRONIZING_BIT_STRING |
                                  ST7920_SPI_RW_WRITE |
                                  ST7920_SPI_RS_INSTRUCTION );                              
                else
                spiSt7920.write( ST7920_SPI_SYNCHRONIZING_BIT_STRING |
                                  ST7920_SPI_RW_WRITE |
                                  ST7920_SPI_RS_DATA );               
            spiSt7920.write( dataBus & 0b11110000 );      
            spiSt7920.write( (dataBus<<4) & 0b11110000 );
            spiSt7920ChipSelect = OFF;
            spiSt7920.unlock();
        break;
    }    
}

//----static-------------------------------------------------------------------
static void displayPinWrite( int pinName, int value )
{
    switch( display.connection ) {
        case DISPLAY_CONNECTION_GPIO_8BITS:
            switch( pinName ) {
                case DISPLAY_PIN_D0: displayD0 = value;   break;
                case DISPLAY_PIN_D1: displayD1 = value;   break;
                case DISPLAY_PIN_D2: displayD2 = value;   break;
                case DISPLAY_PIN_D3: displayD3 = value;   break;
                case DISPLAY_PIN_D4: displayD4 = value;   break;
                case DISPLAY_PIN_D5: displayD5 = value;   break;
                case DISPLAY_PIN_D6: displayD6 = value;   break;
                case DISPLAY_PIN_D7: displayD7 = value;   break;
                case DISPLAY_PIN_RS: displayRs = value;   break;
                case DISPLAY_PIN_EN: displayEn = value;   break;
                case DISPLAY_PIN_RW: break; 
                default: break;
            }
            break;
        case DISPLAY_CONNECTION_GPIO_4BITS:
            switch( pinName ) {
                case DISPLAY_PIN_D4: displayD4 = value;   break;
                case DISPLAY_PIN_D5: displayD5 = value;   break;
                case DISPLAY_PIN_D6: displayD6 = value;   break;
                case DISPLAY_PIN_D7: displayD7 = value;   break;
                case DISPLAY_PIN_RS: displayRs = value;   break;
                case DISPLAY_PIN_EN: displayEn = value;   break;
                case DISPLAY_PIN_RW: break; 
                default: break;
            }
            break;
        case DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER:
           if ( value ) {
                switch( pinName ) {
                    case DISPLAY_PIN_D4: pcf8574.displayPinD4 = ON; break;
                    case DISPLAY_PIN_D5: pcf8574.displayPinD5 = ON; break;
                    case DISPLAY_PIN_D6: pcf8574.displayPinD6 = ON; break;
                    case DISPLAY_PIN_D7: pcf8574.displayPinD7 = ON; break;
                    case DISPLAY_PIN_RS: pcf8574.displayPinRs = ON; break;
                    case DISPLAY_PIN_EN: pcf8574.displayPinEn = ON; break;
                    case DISPLAY_PIN_RW: pcf8574.displayPinRw = ON; break;
                    case DISPLAY_PIN_A_PCF8574: pcf8574.displayPinA = ON; break;
                    default: break;
                }
            }
            else {
                switch( pinName ) {
                    case DISPLAY_PIN_D4: pcf8574.displayPinD4 = OFF; break;
                    case DISPLAY_PIN_D5: pcf8574.displayPinD5 = OFF; break;
                    case DISPLAY_PIN_D6: pcf8574.displayPinD6 = OFF; break;
                    case DISPLAY_PIN_D7: pcf8574.displayPinD7 = OFF; break;
                    case DISPLAY_PIN_RS: pcf8574.displayPinRs = OFF; break;
                    case DISPLAY_PIN_EN: pcf8574.displayPinEn = OFF; break;
                    case DISPLAY_PIN_RW: pcf8574.displayPinRw = OFF; break;
                    case DISPLAY_PIN_A_PCF8574: pcf8574.displayPinA = OFF; break;
                    default: break;
                }
            }     
            pcf8574.data = 0b00000000;
            if ( pcf8574.displayPinRs ) pcf8574.data |= 0b00000001; 
            if ( pcf8574.displayPinRw ) pcf8574.data |= 0b00000010; 
            if ( pcf8574.displayPinEn ) pcf8574.data |= 0b00000100; 
            if ( pcf8574.displayPinA  ) pcf8574.data |= 0b00001000; 
            if ( pcf8574.displayPinD4 ) pcf8574.data |= 0b00010000; 
            if ( pcf8574.displayPinD5 ) pcf8574.data |= 0b00100000; 
            if ( pcf8574.displayPinD6 ) pcf8574.data |= 0b01000000; 
            if ( pcf8574.displayPinD7 ) pcf8574.data |= 0b10000000; 
            i2cPcf8574.write( pcf8574.address, &pcf8574.data, 1);
            break;

        case DISPLAY_CONNECTION_SPI:
        break;
    }
}

//----static-------------------------------------------------------------------
static void displayDataBusWrite( int dataBus )
{
    displayPinWrite( DISPLAY_PIN_EN, OFF );
    displayPinWrite( DISPLAY_PIN_D7, dataBus & 0b10000000 );
    displayPinWrite( DISPLAY_PIN_D6, dataBus & 0b01000000 );
    displayPinWrite( DISPLAY_PIN_D5, dataBus & 0b00100000 );
    displayPinWrite( DISPLAY_PIN_D4, dataBus & 0b00010000 );
    switch( display.connection ) {
        case DISPLAY_CONNECTION_GPIO_8BITS:
            displayPinWrite( DISPLAY_PIN_D3, dataBus & 0b00001000 );
            displayPinWrite( DISPLAY_PIN_D2, dataBus & 0b00000100 );  
            displayPinWrite( DISPLAY_PIN_D1, dataBus & 0b00000010 );      
            displayPinWrite( DISPLAY_PIN_D0, dataBus & 0b00000001 );
        break; 
              
        case DISPLAY_CONNECTION_GPIO_4BITS:
        case DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER:
            if ( initial8BitCommunicationIsCompleted == true) {
                displayPinWrite( DISPLAY_PIN_EN, ON );         
                delay( 1 );
                displayPinWrite( DISPLAY_PIN_EN, OFF );              
                delay( 1 );        
                displayPinWrite( DISPLAY_PIN_D7, dataBus & 0b00001000 );
                displayPinWrite( DISPLAY_PIN_D6, dataBus & 0b00000100 );  
                displayPinWrite( DISPLAY_PIN_D5, dataBus & 0b00000010 );      
                displayPinWrite( DISPLAY_PIN_D4, dataBus & 0b00000001 );                
            }
        break;
        
        case DISPLAY_CONNECTION_SPI:
        break;
    }
    displayPinWrite( DISPLAY_PIN_EN, ON );              
    delay( 1 );
    displayPinWrite( DISPLAY_PIN_EN, OFF );  
    delay( 1 );                   
}

















// /*!****************************************************************************
//  * @file display.cpp
//  * @brief TODO
//  * @author Quattrone Martin
//  * @date Oct 2023
//  *******************************************************************************/

// //=====[Libraries]=============================================================

// #include "mbed.h"
// #include "display.h"

// namespace Drivers {

// //=====[Declaration and initialization of private global variables]============

// Display* Display::mInstance = nullptr;

// //=====[Implementations of public functions]===================================

// //----static-------------------------------------------------------------------
// void Display::Init(int pin)
// {
//     if (mInstance == nullptr)
//     {
//         mInstance = new Display(pin);
//     }
// }

// //----static-------------------------------------------------------------------
// Display* Display::GetInstance()
// {
//     return mInstance;
// }

// //=====[Implementations of private functions]==================================

// } // namespace Drivers
