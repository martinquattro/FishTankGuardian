/*!****************************************************************************
 * @file memory.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "memory.h"

#include "arm_book_lib.h"

namespace Util {

//=====[Declaration and initialization of private global variables]============

Memory* Memory::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void Memory::Init()
{
    DEBUG_PRINT("Memory::Init() - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new Memory(EEPROM_PIN_SDA, EEPROM_PIN_SCL, EEPROM_ADDRESS_ID, EEPROM_TYPE);
    }
}

//----static-------------------------------------------------------------------
Memory* Memory::GetInstance()
{
    return mInstance;
}

//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
Memory::Memory(PinName sdaPin, PinName sclPin, uint8_t address, EEPROM::TypeEeprom eepromType)
    : mEeprom(sdaPin, sclPin, address, EEPROM::T24C64)
{

}

} // namespace Util

