#include <iostream>
#include "cpu.h"
using namespace std;

cpu::cpu(vector<uint32_t> instr)
{
    this->instr = instr;
    PC = 0;
}

void cpu::run()
{
    for (int i = 0; i < instr.size(); i++)
    {
        uint8_t opcode = getOpcode(instr[i]);
        switch (opcode)
        {
        case R:
            r_type(instr[i]);
            PC += 4;
            break;
        case I:
            i_type(instr[i]);
            PC += 4;
            break;
        case S:
            s_type(instr[i]);
            PC += 4;
            break;
        case L:
            l_type(instr[i]);
            PC += 4;
            break;
        case B:
            b_type(instr[i]);
            break;
        case JAL:
            jal(instr[i]);
            break;
        case JALR:
            jalr(instr[i]);
            break;
        case LUI:
            lui(instr[i]);
            break;
        }
    }

    // switch case for instruction type

    // convert to String
}

// USER OPTIONS FUNCTIONS
uint32_t cpu::getPC()
{
    return PC;
}
uint32_t cpu::getReg(uint8_t index)
{
    return reg.readReg(index);
}
uint32_t cpu::getMem(uint32_t addr)
{
    return mem.get_mem(addr);
}
string cpu::convertASM(uint8_t op, uint8_t in1, uint8_t in2)
{
    return "";
}

// DECODE FUNCTIONS
uint8_t cpu::getOpcode(uint32_t instr)
{
    return (uint8_t)instr & 0b01111111;
}
uint8_t cpu::getrd(uint32_t instr)
{
    return (uint8_t)(instr >> 7) & 0b00011111;
}
uint8_t cpu::getrs1(uint32_t instr)
{
    return (uint8_t)(instr >> 15) & 0b00011111;
}
uint8_t cpu::getrs2(uint32_t instr)
{
    return (uint8_t)(instr >> 20) & 0b00011111;
}
uint8_t cpu::getfunct3(uint32_t instr)
{
    return (uint8_t)(instr >> 12) & 0b00000111;
}
uint8_t cpu::getfunct7(uint32_t instr)
{
    return (uint8_t)(instr >> 25) & 0b01111111;
}
uint16_t cpu::getimm12(uint32_t instr)
{
    uint16_t val = (uint16_t)(instr >> 20) & 0b0111111111111;
    if (val & 0b100000000000)               // check msb
        return (val | 0b1111000000000000);  // if (-), sign-extend
    return val;
}
uint8_t cpu::getALU_op(uint32_t instr)
{
    uint8_t funct3 = getfunct3(instr);
    uint8_t funct7 = getfunct7(instr);
    return funct3 | funct7;
}

void cpu::r_type(uint32_t instr)
{
    cout << "R-type" << endl; // DEBUG
    uint8_t alu_op = getALU_op(instr);
    cout << "aluOp:" << static_cast<int>(alu_op);
    uint8_t rd = getrd(instr);
    uint8_t rs1 = getrs1(instr);
    uint8_t rs2 = getrs2(instr);

    uint32_t val1 = reg.readReg(rs1); // read from reg
    uint32_t val2 = reg.readReg(rs2);
    uint32_t result = alu.calculate(val1, val2, alu_op); // execute
    reg.writeReg(rd, result);                            // write to reg
}
void cpu::i_type(uint32_t instr)
{
    cout << "I-type" << endl; // DEBUG
    // delete, issue w/ include "alu.h"
    const static uint8_t SLLI = 0b00000001;
    const static uint8_t SRLI = 0b00001001;
    const static uint8_t SRAI = 0b00000011;
    //
    uint8_t alu_op = getfunct3(instr) | 0b00000000;
    uint8_t rd = getrd(instr);
    uint8_t rs1 = getrs1(instr);
    uint8_t rs2 = getrs2(instr); //only for shift instructions

    uint32_t val1 = reg.readReg(rs1); // read from reg
    uint16_t val2 = getimm12(instr);
    if (alu_op == SLLI || alu_op == SRLI || alu_op == SRAI)
        val2 = reg.readReg(rs2); // getShamt()

    uint32_t result = alu.calculate(val1, (uint32_t)val2, alu_op); // execute
    reg.writeReg(rd, result);                            // write to reg

    // DEBUG
    // cout << "rd:" << static_cast<int>(rd) << " rs1:" << static_cast<int>(rs1) << " rs2:" << static_cast<int>(val2) << endl;
    // cout << "val1:" << static_cast<int>(val1) << " val2:" << static_cast<int>(val2) << " result:" << static_cast<int>(result) << endl;
    // cout << "(binary)val2:" <<bitset<sizeof(int) * 8>(val2)<<endl;
}

void cpu::s_type(uint32_t instr)
{
}
void cpu::b_type(uint32_t instr)
{
}
void cpu::l_type(uint32_t instr)
{
}
void cpu::jal(uint32_t instr)
{
}
void cpu::jalr(uint32_t instr)
{
}
void cpu::lui(uint32_t instr)
{
}
void cpu::auipc(uint32_t instr)
{
}