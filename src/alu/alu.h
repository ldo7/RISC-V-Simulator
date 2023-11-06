#include <iostream>
#ifndef RISCV_SIMULATOR_ALU_H
#define RISCV_SIMULATOR_ALU_H

using namespace std;

class ALU {

public:
    uint32_t calculate(uint32_t, uint32_t, uint8_t);
};


#endif //RISCV_SIMULATOR_ALU_H