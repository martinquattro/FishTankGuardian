#include <vector>
#include "mbed.h"
#include "memory.h"

//=====[Implementations of public methods]=====================================

//-----------------------------------------------------------------------------
Memory::Memory(PinName SDA, PinName SCL, uint8_t address):
    mEeprom(SDA, SCL), mI2cAddress(address << 1)
{

}

//-----------------------------------------------------------------------------
bool Memory::Write(uint16_t address, float number)
{
    std::vector<uint8_t> vector(sizeof(float));

    memcpy(&vector[0], &number, sizeof(float));

    return _Write(address, vector) == sizeof(float);
}

//-----------------------------------------------------------------------------
bool Memory::Read(uint16_t address, float &number)
{
    std::vector<uint8_t> vector(sizeof(float));

    if (_Read(address, vector) != sizeof(float))
    {
        return false;
    }

    memcpy(&number, &vector[0], sizeof(float));

    return true;
}

//-----------------------------------------------------------------------------
bool Memory::Write(uint16_t address, bool boolean)
{
    std::vector<uint8_t> vector(sizeof(bool));

    memcpy(&vector[0], &boolean, sizeof(bool));

    return _Write(address, vector) == sizeof(bool);
}

//-----------------------------------------------------------------------------
bool Memory::Read(uint16_t address, bool &boolean)
{
    std::vector<uint8_t> vector(sizeof(bool));

    if (_Read(address, vector) != sizeof(bool))
    {
        return false;
    }

    memcpy(&boolean, &vector[0], sizeof(bool));

    return true;
}

//-----------------------------------------------------------------------------
bool Memory::Write(uint16_t address, int number)
{
    std::vector<uint8_t> vector(sizeof(int));

    memcpy(&vector[0], reinterpret_cast<uint8_t *>(&number), sizeof(int));

    return _Write(address, vector) == sizeof(int);
}

//-----------------------------------------------------------------------------
bool Memory::Read(uint16_t address, int &number) 
{
    std::vector<uint8_t> vector(sizeof(int));

    if (_Read(address, vector) != sizeof(int))
    {
        return false;
    }

    memcpy(&number, &vector[0], sizeof(int));

    return true;
}

//-----------------------------------------------------------------------------
bool Memory::Write(uint16_t address, uint8_t number)
{
    std::vector<uint8_t> vector(sizeof(uint8_t));

    memcpy(&vector[0], reinterpret_cast<uint8_t *>(&number), sizeof(uint8_t));

    return _Write(address, vector) == sizeof(uint8_t);
}

//-----------------------------------------------------------------------------
bool Memory::Read(uint16_t address, uint8_t &number)
{
    std::vector<uint8_t> vector(sizeof(uint8_t));

    if (_Read(address, vector) != sizeof(uint8_t))
        return false;

    memcpy(&number, &vector[0], sizeof(uint8_t));

    return true;
}

//-----------------------------------------------------------------------------
bool Memory::Write(uint16_t address, uint32_t number)
{
    std::vector<uint8_t> vector(sizeof(uint32_t));

    memcpy(&vector[0], reinterpret_cast<uint8_t *>(&number), sizeof(uint32_t));

    return _Write(address, vector) == sizeof(uint32_t);
}

//-----------------------------------------------------------------------------
bool Memory::Read(uint16_t address, uint32_t &number)
{
    std::vector<uint8_t> vector(sizeof(uint32_t));

    if (_Read(address, vector) != sizeof(uint32_t))
        return false;

    memcpy(&number, &vector[0], sizeof(uint32_t));

    return true;
}

//-----------------------------------------------------------------------------
bool Memory::Write(uint16_t address, char character)
{
    std::vector<uint8_t> vector(sizeof(char));

    memcpy(&vector[0], reinterpret_cast<uint8_t *>(&character), sizeof(char));

    return _Write(address, vector) == sizeof(char);
}

//-----------------------------------------------------------------------------
bool Memory::Read(uint16_t address, char &character)
{
    std::vector<uint8_t> vector(sizeof(char));

    if (_Read(address, vector) != sizeof(char))
        return false;

    memcpy(&character, &vector[0], sizeof(char));

    return true;
}

//-----------------------------------------------------------------------------
bool Memory::Write(uint16_t address, const std::string &string)
{
    size_t length = string.length();
    std::vector<uint8_t> vector(length + 1);
    vector[length] = 0;

    memcpy(&vector[0], string.c_str(), length);

    return _Write(address, vector) == length + 1;
}

//-----------------------------------------------------------------------------
bool Memory::Read(uint16_t address, std::string &string)
{
    std::vector<uint8_t> vector(MAX_PAGE_SIZE);

    if (_Read(address, vector) != MAX_PAGE_SIZE)
    {
        return false;
    }

    for (auto c : vector) 
    {
        if (!c)
        {
            break;
        }
        string += c;
    }

    return true;
}

//=====[Implementations of private methods]====================================

//----private------------------------------------------------------------------
uint16_t Memory::_Write(uint16_t address, const std::vector<uint8_t> &vector)
{
    size_t write_length = vector.size();

    if (address > MAX_ADDRESS)
        return 0;
    if (write_length > (MAX_ADDRESS - address + 1))
        write_length = MAX_ADDRESS - address + 1;
    if (write_length > MAX_PAGE_SIZE)
        return 0;

    uint16_t count = 0;

    mEeprom.start();
    count += mEeprom.write(mI2cAddress | 0);
    count += mEeprom.write(address >> 8 & 0x0F);
    count += mEeprom.write(address & 0xFF);

    for (uint16_t i = 0; i < write_length; i++)
        count += mEeprom.write(vector[i]);

    mEeprom.stop();

    if (count != write_length + 3)
        return 0;

    ThisThread::sleep_for(WRITE_TIME);

    return write_length;
}

//----private------------------------------------------------------------------
uint16_t Memory::_Read(uint16_t address, std::vector<uint8_t> &vector)
{
    size_t read_length = vector.size();

    if (address > MAX_ADDRESS)
        return 0;
    if (read_length > (MAX_ADDRESS - address + 1))
        read_length = MAX_ADDRESS - address + 1;
    if (read_length > MAX_PAGE_SIZE)
        return 0;

    uint16_t count = 0;

    mEeprom.start();
    count += mEeprom.write(mI2cAddress | 0);
    count += mEeprom.write(address >> 8 & 0x0F);
    count += mEeprom.write(address & 0xFF);
    mEeprom.start();
    count += mEeprom.write(mI2cAddress | 1);
    for (uint16_t i = 0; i < read_length - 1; i++)
        vector[i] = mEeprom.read(1);
    vector[read_length - 1] = mEeprom.read(0);
    mEeprom.stop();

    if (count != 4)
    {
        return 0;
    }

    return read_length;
}

//----private------------------------------------------------------------------
std::vector<uint8_t> Memory::_Read(uint16_t address_from, uint16_t address_to) 
{
    size_t read_length = address_to - address_from;

    std::vector<uint8_t> vector(read_length);

    if (address_from > MAX_ADDRESS)
        return std::vector<uint8_t>();
    if (address_to > address_from)
        return std::vector<uint8_t>();
    if (read_length > (MAX_ADDRESS - address_from + 1))
        read_length = MAX_ADDRESS - address_from + 1;
    if (read_length > MAX_PAGE_SIZE)
        return std::vector<uint8_t>();

    uint16_t count = 0;

    mEeprom.start();
    count += mEeprom.write(mI2cAddress | 0);
    count += mEeprom.write(address_from >> 8 & 0x0F);
    count += mEeprom.write(address_from & 0xFF);
    mEeprom.start();
    count += mEeprom.write(mI2cAddress | 1);
    for (uint16_t i = 0; i < read_length - 1; i++)
    {
        vector[i] = mEeprom.read(1);
    }
    vector[read_length - 1] = mEeprom.read(0);
    mEeprom.stop();

    if (count != 4)
    {
        return std::vector<uint8_t>();
    }

    return vector;
}