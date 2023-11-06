// #include <iostream>

//     uint32_t alu::calculate(uint32_t in1, uint32_t in2, uint8_t op) { 
//     {
//         if (op == "add") {
//             return input1 + input2;
//         } else if (op == "sub") {
//             return input1 - input2;
//         } else if (op == "and") {
//             return input1 & input2;
//         } else if (op == "or") {
//             return input1 | input2;
//         } else if (op == "xor") {
//             return input1 ^ input2;
//         } else if (op == "sll") {
//             return input1 << input2;
//         } else if (op == "srl") {
//             // C++ does not have a logical right shift operator. The arithmetic right shift operator will work correctly for non-negative numbers.
//             return input1 >> input2;
//         } else if (op == "sra") {
//             return input1 >> input2;
//         } else if (op == "eq") {
//             return input1 == input2;
//         } else if (op == "ne") {
//             return input1 != input2;
//         } else if (op == "lt") {
//             return input1 < input2;
//         } else if (op == "ge") {
//             return input1 >= input2;
//         } else {
//             std::cerr << "Unsupported ALU operation: " << op << std::endl;
//             return 0;
//         }
//     }
//     }
// }
    
// // Example usage
// int main() 
// {
//     RiscVProcessor processor;

//     // Execute some instructions
//     processor.executeInstruction("LUI 1 100");  // Load Upper Immediate
//     processor.executeInstruction("ADDI 2 1 50");  // Add Immediate
//     processor.executeInstruction("SUB 3 2 1");  // Subtract
//     processor.executeInstruction("AND 4 2 3");  // Bitwise AND
//     processor.executeInstruction("OR 5 2 3");  // Bitwise OR
//     processor.executeInstruction("XOR 6 2 3");  // Bitwise XOR
//     processor.executeInstruction("SLL 7 2 3");  // Shift Left Logical
//     processor.executeInstruction("SRL 8 2 3");  // Shift Right Logical
//     processor.executeInstruction("SRA 9 2 3");  // Shift Right Arithmetic
//     processor.executeInstruction("BEQ 10 11 offset");   // Branch Equal
//     processor.executeInstruction("BNE 12 13 offset");   // Branch Not Equal

//     // Calculate using the ALU
//     int result = processor.calculateALU("add", 5, 3);
//     std::cout << "ALU Result: " << result << std::endl;

//     return 0;
// }


//changed based on Loni's review

#include <cstdint>
#include "alu.h"

uint32_t alu::calculate(uint32_t in1, uint32_t in2, uint8_t op) {
    uint32_t out;

    if (op == ALU_OP_ADD) {
        out = in1 + in2;
    } else if (op == ALU_OP_SLL) {
        out = in1 << in2;
    } else if (op == ALU_OP_XOR) {
        out = in1 ^ in2;
    } else if (op == ALU_OP_OR) {
        out = in1 | in2;
    } else if (op == ALU_OP_AND) {
        out = in1 & in2;
    } else if (op == ALU_OP_SRL) {
        out = in1 >> in2; // Shift Right Logical
    } else if (op == ALU_OP_SUB) {
        out = in1 - in2;
    } else if (op == ALU_OP_SRA) {
        out = ((int32_t)in1) >> in2; // Shift Right Arithmetic
    } else if (op == ALU_OP_SLT) {
        out = (uint32_t)0 | ((int32_t)in1) < ((int32_t)in2); // Set If Less Than (Signed)
    } else if (op == ALU_OP_SLTU) {
        out = (uint32_t)0 | (in1 < in2); // Set If Less Than (Unsigned)
    } else {
        out = (uint32_t)0;
        std::cout << "ALU: invalid operating instruction" << std::endl;
    }

    return out;
}
