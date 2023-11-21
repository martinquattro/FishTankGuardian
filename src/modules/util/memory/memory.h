/*!****************************************************************************
 * @file memory.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef MEMORY
#define MEMORY

#define EEPROM_PIN_SDA        PB_11
#define EEPROM_PIN_SCL        PB_10
#define EEPROM_ADDRESS_ID     0x68
#define EEPROM_TYPE           EEPROM::T24C64

#include "mbed.h"
#include "EEPROM.h"

namespace Util { 

    class Memory 
    {
        public:

            //! Initialize the rtc
            static void Init();

            //! Returns the rtc object
            static Memory* GetInstance();

        private:

            Memory(PinName sdaPin, PinName sclPin, uint8_t address, EEPROM::TypeEeprom eepromType);
            ~Memory() = default;
            Memory(const Memory&) = delete;
            Memory& operator=(const Memory&) = delete;

            static Memory* mInstance;
            EEPROM mEeprom;
    };
} // namespace Util

#endif // MEMORY