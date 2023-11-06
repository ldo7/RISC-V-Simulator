#include <cstdint>

#ifndef REGISTER_H
#define REGISTER_H

class Register {
public:
    Register();  
    uint32_t readReg(uint8_t index) const;          // Read a 32-bit register
    void writeReg(uint8_t index, uint32_t value);  // Write a 32-bit register
    // void reset();                                   // Reset all registers to zero
private:
    static const int NUM_REGISTERS = 32;
    uint32_t registers[NUM_REGISTERS];
    // array<uint32_t, 32> registers;
};

#endif 
