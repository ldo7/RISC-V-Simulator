#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#ifndef __ALU_H
#define __ALU_H

class alu
{
public:
    int32_t calculate(int32_t in1, int32_t in2, int8_t op);

private:
    const static int8_t ADD = 0b00000000; // alu opcodes
    const static int8_t SUB = 0b00100000;
    const static int8_t OR = 0b00000110; 
    const static int8_t AND = 0b00000111;
    const static int8_t XOR = 0b00000100;
    const static int8_t SRL = 0b00000101;
    const static int8_t SRA = 0b00100101;
    const static int8_t SLL = 0b00000001;
    const static int8_t SLT = 0b00000010;
    const static int8_t SLTU = 0b00000011;
};
#endif