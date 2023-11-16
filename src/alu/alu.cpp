#include "alu.h"

uint32_t alu::calculate(uint32_t in1, uint32_t in2, uint8_t op){
    uint32_t out;
    if (op == ADD) {  //add
        out = in1 + in2;
    } else if (op == SLL) {  // shift left logical
        out = in1 << in2;
    } else if (op == XOR) {  // exclusive or
        out = in1 ^ in2;
    } else if (op == OR) {   //or
        out = in1 | in2;
    } else if (op == AND) {  //AND 
        out = in1 & in2;
    } else if (op == SRL) {  // shift right logical 
        out = in1 >> in2; 
    } else if (op == SUB) {  //subtract
        out = in1 - in2;
    } else if (op == SRA) { 
        out = ((int32_t)in1) >> in2; // Shift Right Arithmetic
    } else if (op == SLT) {
        out = (uint32_t)0 | ((in1) < (in2)); // Set If Less Than (Signed)
    } else if (op == SLTU) {
        out = (uint32_t)0 | (in1 < in2); // Set If Less Than (Unsigned)
    } else {
        out = (uint32_t)0;
        std::cout << "ALU: invalid operating instruction" << std::endl;
    }
    return out;
}