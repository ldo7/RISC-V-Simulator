#include <iostream>
#include <cstdint>
#include <bitset>

class alu
{
public:
    int32_t calculate(int16_t in1, int16_t in2, int8_t op);
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

int32_t alu::calculate(int16_t in1, int16_t in2, int8_t op){
    int32_t out; 
    switch (op) {
        case ADD: out = in1 + in2; break;
        case SUB: out = in1 - in2; break;
        case XOR: out = in1 ^ in2; break;
        case OR: out = in1 | in2; break;
        case AND: out = in1 & in2; break;
        case SLL: out = (uint32_t)in1 << (uint32_t)in2; break;
        case SRL: out = (uint32_t)in1 >> (uint32_t)in2; break;
        case SRA: out = in1 >> in2; break;
        case SLT: out = 0 | (in1 < in2); break;
        case SLTU: out = 0 | ((uint32_t)in1 < (uint32_t)in2); break;
        default: out = 0; std::cout << "ALU: invalid aluOp" << std::endl;
    }
    return out;
}