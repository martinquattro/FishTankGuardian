#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <vector>
#include "mbed.h"

#define MAX_ADDRESS     0x0FFF
#define MAX_PAGE_SIZE   32
#define WRITE_TIME      20ms

class Memory {

    public:
        Memory(PinName, PinName, uint8_t);

        // Writes a float in memory.
        bool Write(uint16_t, float);

        // Reads a float from memory.
        bool Read(uint16_t, float &);

        // Writes a bool in memory.
        bool Write(uint16_t, bool);

        // Reads a bool from memory.
        bool Read(uint16_t, bool &);

        // Writes a int in memory.
        bool Write(uint16_t, int);

        // Reads a int from memory.
        bool Read(uint16_t, int &);

        // Writes a uint8_t in memory.
        bool Write(uint16_t, uint8_t);

        // Reads a uint8_t in memory.
        bool Read(uint16_t, uint8_t&);

        // Writes a uint32_t in memory.
        bool Write(uint16_t, uint32_t);

        // Reads a uin32_t from memory.
        bool Read(uint16_t, uint32_t&);

        // Writes a char in memory.
        bool Write(uint16_t, char);

        // Reads a char from memory.
        bool Read(uint16_t, char&);

        // Writes a string in memory.
        bool Write(uint16_t, const std::string &);

        // Reads a string from memory.
        bool Read(uint16_t, std::string &);

    private:

        uint16_t _Write(uint16_t, const std::vector<uint8_t> &);

        uint16_t _Read(uint16_t, std::vector<uint8_t> &);

        std::vector<uint8_t> _Read(uint16_t address_from, uint16_t address_to);

        I2C mEeprom;
        uint8_t mI2cAddress;
};

#endif  // MEMORY_H


