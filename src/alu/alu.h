#include <iostream>
#include <vector>
#include <string>

#ifndef ALU_H
#define ALU_H

class alu
{
public:
    alu();
    void executeInstruction(std::string instruction);
    int calculateALU(std::string op, int input1, int input2);

private:
    int mapOperationToCode(const std::string &op);
    int mapALUOperationToCode(const std::string &op);

    std::vector<int> registers; // 32 general-purpose registers
    // Define unique integer values for instructions
    static const int LUI = 1;
    static const int ADDI = 2;
    static const int SUB = 3;
    static const int AND = 4;
    static const int OR = 5;
    static const int XOR = 6;
    static const int SLL = 7;
    static const int SRL = 8;
    static const int SRA = 9;
    static const int BEQ = 10;
    static const int BNE = 11;
    // Define unique integer values for ALU operations
    static const int ADD = 101;
    static const int SUB_ALU = 102;
    static const int AND_ALU = 103;
    static const int OR_ALU = 104;
    static const int XOR_ALU = 105;
    static const int SLL_ALU = 106;
    static const int SRL_ALU = 107;
    static const int SRA_ALU = 108;
    static const int EQ = 109;
    static const int NE = 110;
    static const int LT = 111;
    static const int GE = 112;
};
#endif