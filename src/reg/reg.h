#include <cstdint>

#ifndef __REG_H
#define __REG_H

class reg
{
public:
    reg();
    uint32_t readReg(int8_t index) const;        // Read a 32-bit register
    void writeReg(uint8_t index, int32_t value); // Write a 32-bit register
    // void reset();                                   // Reset all registers to zero
private:
    static const int NUM_REGISTERS = 32;
    int32_t registers[NUM_REGISTERS];
    // array<uint32_t, 32> registers;
};

#endif
