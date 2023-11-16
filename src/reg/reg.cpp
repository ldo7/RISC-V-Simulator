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
uint32_t reg::readReg(uint8_t rd) const
{
    if (rd == 0)
        return 0; // x0 is hardwired to zero
    return registers[rd];
}

// Write a value to register rd
void reg::writeReg(uint8_t rd, uint32_t value)
{
    if (rd != 0)
    {
        // Avoid writing to register x0
        registers[rd] = value;
    }
}

// int main()
// {
//     reg registerFile;

//     // test: Write a value to register x10
//     registerFile.writeReg(10, 42);

//     // tset: Read the value from register x10
//     uint32_t value = registerFile.readReg(10);
//     std::cout << "Value in x10: " << value << endl;

//     return 0;
// }
