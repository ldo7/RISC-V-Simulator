#include <iostream>
<<<<<<< HEAD
#include "alu.h"

alu::alu() : registers(32, 0) {}

void alu::executeInstruction(std::string instruction)
{
    // Parse the instruction and extract relevant fields
    // This is a placeholder. In a real implementation, you would parse the instruction string to extract these fields.
    std::string op;
    int rd, rs1, rs2, imm;

    // Map operation strings to unique integer values
    int opCode = mapOperationToCode(op);

    switch (opCode)
    {
    case LUI:
        registers[rd] = imm;
        break;
    case ADDI:
        registers[rd] = registers[rs1] + imm;
        break;
    case SUB:
        registers[rd] = registers[rs1] - registers[rs2];
        break;
    case AND:
        registers[rd] = registers[rs1] & registers[rs2];
        break;
    case OR:
        registers[rd] = registers[rs1] | registers[rs2];
        break;
    case XOR:
        registers[rd] = registers[rs1] ^ registers[rs2];
        break;
    case SLL:
        registers[rd] = registers[rs1] << registers[rs2];
        break;
    case SRL:
        registers[rd] = registers[rs1] >> registers[rs2];
        break;
    case SRA:
        registers[rd] = registers[rs1] >> registers[rs2];
        break;
    case BEQ:
        if (registers[rs1] == registers[rs2])
        {
            // Implement branch logic here
        }
        break;
    case BNE:
        if (registers[rs1] != registers[rs2])
        {
            // Implement branch logic here
        }
        break;
    default:
        std::cerr << "Unsupported instruction: " << op << std::endl;
    }
}

int alu::calculateALU(std::string op, int input1, int input2)
{
    // Map ALU operation strings to unique integer values
    int opCode = mapALUOperationToCode(op);

    switch (opCode)
    {
    case ADD:
        return input1 + input2;
    case SUB:
        return input1 - input2;
    case AND:
        return input1 & input2;
    case OR:
        return input1 | input2;
    case XOR:
        return input1 ^ input2;
    case SLL:
        return input1 << input2;
    case SRL:
        return input1 >> input2;
    case SRA:
        return input1 >> input2;
    case EQ:
        return input1 == input2;
    case NE:
        return input1 != input2;
    case LT:
        return input1 < input2;
    case GE:
        return input1 >= input2;
    default:
        std::cerr << "Unsupported ALU operation: " << op << std::endl;
        return 0;
    }
}

int alu::mapOperationToCode(const std::string &op)
{
    // Map operation strings to unique integer values
    if (op == "LUI")
        return LUI;
    else if (op == "ADDI")
        return ADDI;
    else if (op == "SUB")
        return SUB;
    else if (op == "AND")
        return AND;
    else if (op == "OR")
        return OR;
    else if (op == "XOR")
        return XOR;
    else if (op == "SLL")
        return SLL;
    else if (op == "SRL")
        return SRL;
    else if (op == "SRA")
        return SRA;
    else if (op == "BEQ")
        return BEQ;
    else if (op == "BNE")
        return BNE;
    return 0; // Unsupported
}

int alu::mapALUOperationToCode(const std::string &op)
{
    // Map ALU operation strings to unique integer values
    if (op == "add")
        return ADD;
    else if (op == "sub")
        return SUB_ALU;
    else if (op == "and")
        return AND_ALU;
    else if (op == "or")
        return OR_ALU;
    else if (op == "xor")
        return XOR_ALU;
    else if (op == "sll")
        return SLL_ALU;
    else if (op == "srl")
        return SRL_ALU;
    else if (op == "sra")
        return SRA_ALU;
    else if (op == "eq")
        return EQ;
    else if (op == "ne")
        return NE;
    else if (op == "lt")
        return LT;
    else if (op == "ge")
        return GE;
    return 0; // Unsupported
}

// Example usage
int main()
{
    alu alu;

<<<<<<< HEAD
    // Execute instructions
=======
    // Execute some instructions
>>>>>>> main
    alu.executeInstruction("LUI 1 100");        // Load Upper Immediate
    alu.executeInstruction("ADDI 2 1 50");      // Add Immediate
    alu.executeInstruction("SUB 3 2 1");        // Subtract
    alu.executeInstruction("AND 4 2 3");        // Bitwise AND
    alu.executeInstruction("OR 5 2 3");         // Bitwise OR
    alu.executeInstruction("XOR 6 2 3");        // Bitwise XOR
    alu.executeInstruction("SLL 7 2 3");        // Shift Left Logical
    alu.executeInstruction("SRL 8 2 3");        // Shift Right Logical
    alu.executeInstruction("SRA 9 2 3");        // Shift Right Arithmetic
    alu.executeInstruction("BEQ 10 11 offset"); // Branch Equal
    alu.executeInstruction("BNE 12 13 offset"); // Branch Not Equal

    // Calculate using the ALU
    int result = alu.calculateALU("add", 5, 3);
    std::cout << "ALU Result: " << result << std::endl;

    return 0;
}
=======
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
>>>>>>> origin/main
