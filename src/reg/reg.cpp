#include <iostream>
#include <array>
#include "reg.h"
using namespace std;

reg::reg()
{
    // Initialize the registers, x0 is hardwired to zero.
    for (int i = 0; i < 32; i++)
    {
        registers[i] = 0;
    }
}

// Read the value from register rd
uint32_t reg::readReg(int8_t rd) const
{
    if (rd == 0)
        return 0; // x0 is hardwired to zero
    return registers[rd];
}

// Write a value to register rd
void reg::writeReg(uint8_t rd, int32_t value)
{
    if (rd != 0)
    {
        // Avoid writing to register x0
        registers[rd] = value;
    }
}
