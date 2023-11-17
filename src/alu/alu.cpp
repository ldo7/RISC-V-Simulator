#include "alu.h"
using namespace std;

int32_t alu::calculate(int32_t in1, int32_t in2, int8_t op){
    int32_t out;
    if (op == ADD) {  //add
        out = in1 + in2;
    } else if (op == SUB) {  //subtract
        out = in1 - in2;
    } else if (op == XOR) {  // exclusive or
        out = in1 ^ in2;
    } else if (op == OR) {   // or
        out = in1 | in2;
    } else if (op == AND) {  // and 
        out = in1 & in2;
    } else if (op == SLL) {  // shift left logical
        out = (uint32_t)in1 << (uint32_t)in2;
    } else if (op == SRL) {  // shift right logical 
        out = (uint32_t)in1 >> (uint32_t)in2; 
    } else if (op == SRA) { 
        out = in1 >> in2; // Shift Right Arithmetic
    } else if (op == SLT) {
        out = (int32_t)0 | ((in1) < (in2)); // Set If Less Than (Signed)
    } else if (op == SLTU) {
        out = (int32_t)0 | (in1 < in2); // Set If Less Than (Unsigned)
    } else {
        out = (int32_t)0;
        cout << "ALU: invalid aluOp" << endl;
    }
    return out;
}