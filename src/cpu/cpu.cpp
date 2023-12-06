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
        case AUIPC:
            auipc(instr[i]);
            break;
        }
    }
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
int16_t cpu::getimm12(uint32_t instr)
{
    int16_t val = (int16_t)(instr >> 20) & 0b0111111111111;
    if (val & 0b100000000000)               // check msb
        return (val | 0b1111000000000000);  // if (-), sign-extend
    return val;
}
uint8_t cpu::getALU_op(uint32_t instr)
{
    uint8_t funct3 = getfunct3(instr);
    uint8_t funct7 = getfunct7(instr);

    if (funct7 == 0b0100000)
        return funct3 | funct7;
    return funct3;
}
uint32_t cpu::get_branch_imm(uint32_t instr)
{
    uint32_t imm = 0;
    uint32_t imm_0 = 0;
    uint32_t imm_11 = (instr >> 7) & 0b0000000;
    uint32_t imm_4_1 = (instr >> 8) & 0b00001111;
    uint32_t imm_10_5 = (instr >> 25) & 0b0000000000000;
    uint32_t imm_12 = (instr >> 31) & 0b0000000000000000000000000000;

    // Combine the extracted bits to get the 13-bit immediate
    imm = imm_0 | imm_11 | imm_4_1 | imm_10_5 | imm_12;

    // add sign extension if imm is negative
    if ((imm_12 >> 12) != 0){
        imm = imm | 0xffffe000; 
    }
    
    return imm;
}
uint32_t cpu::get_jal_offset(uint32_t instr)
{
    uint32_t offset = 0;
    uint32_t offset_0 = 0;
    uint32_t offset20 = (instr & 0x80000000) >> 11;
    uint32_t offset10_1 = (instr & 0b01111111111000000000000000000000) >> 20;
    uint32_t offset11 =    (instr & 0b00000000000100000000000000000000) >> 9;
    uint32_t offset19_12= (instr & 0b00000000000011111111000000000000);

    // Combine the extracted bits to get the 21-bit offset
    offset = offset20 | offset19_12 | offset11 | offset10_1 | offset_0;

    // add sign extension if the offset is negative
    if ((offset20 >> 20) != 0){
        offset = offset | 0xfff00000; 
    }
    return offset;
}

void cpu::r_type(uint32_t instr)
{
    uint8_t alu_op = getALU_op(instr);
    uint8_t rd = getrd(instr);
    uint8_t rs1 = getrs1(instr);
    int8_t rs2 = getrs2(instr);

    uint16_t val1 = reg.readReg(rs1);                   // read from reg
    uint16_t val2 = reg.readReg(rs2);
    uint32_t result = alu.calculate(val1, val2, alu_op); // execute
    reg.writeReg(rd, result);                            // write to reg

    // DEBUG
    cout << stringify(instr,rd,rs1,rs2)<< endl;
    cout << "val1:" << static_cast<int>(val1) << " val2:" << static_cast<int>(val2) << " result:" << static_cast<int>(result) << endl;
}
void cpu::i_type(uint32_t instr)
{
    uint8_t alu_op = getALU_op(instr);
    uint8_t rd = getrd(instr);
    uint8_t rs1 = getrs1(instr);
    int8_t rs2 = getrs2(instr); // only for shift instructions

    int16_t val1 = reg.readReg(rs1); // read from reg
    int16_t val2 = getimm12(instr);
    if (alu_op == SLL || alu_op == SRL || alu_op == SRA)
        val2 = rs2; // getShamt()

    cout << stringify(instr, rd, rs1, val2) << endl;
    int32_t result = alu.calculate(val1, val2, alu_op); // execute
    reg.writeReg(rd, result);                           // write to reg

    // DEBUG
    cout << "val1:" << static_cast<int>(val1) << " val2:" << static_cast<int>(val2) << " result:" << static_cast<int>(result) << endl;
    // cout << "(binary)result:" << bitset<32>(result)<<endl;
    // cout << "register: "<<reg.readReg(rd)<<endl;   //check if stored properly
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

//convert binary to asm string representation
string cpu::stringify(int32_t instr, int8_t rd,int8_t rs1,int16_t rs2)
{
    string str="";
    uint8_t opcode = getOpcode(instr);
    uint8_t aluOp = getALU_op(instr);
    uint8_t funct3 = getfunct3(instr);
    uint8_t funct7 = getfunct7(instr);
    
    switch(opcode){
        case R:
        case I:
            switch (aluOp) {
                case ADD: str = "add"; break;
                case SUB: str = "sub"; break;
                case XOR: str = "xor"; break;
                case OR: str = "or"; break;
                case AND: str = "and"; break;
                case SLL: str = "sll"; break;
                case SRL: str = "srl"; break;
                case SRA: str = "sra"; break;
                case SLT: str = "slt"; break;
                case SLTU: str = "sltu"; break;
                default: str = "invalid aluOp"; 
            }
            if (opcode == I) str+="i";
            break;
        case L:
            switch (funct3) {
                case 0b000: str = "lb"; break;
                case 0b001: str = "lh"; break;
                case 0b010: str = "lw"; break;
                case 0b100: str = "lbu"; break;
                case 0b101: str = "lhu"; break;
                default: str = "invalid L-type"; break;
            }
            break;
        case S:
            switch (funct3) {
                case 0b000: str = "sb"; break;
                case 0b001: str = "sh"; break;
                case 0b010: str = "sw"; break;
                default: str = "unknown S-type"; break;
            }
            break;
        case B:
            switch (funct3) {
                case 0b000: str = "beq"; break;
                case 0b001: str = "bne"; break;
                case 0b100: str = "blt"; break;
                case 0b101: str = "bge"; break;
                case 0b110: str = "bltu"; break;
                case 0b111: str = "bgeu"; break;
                default: str = "unknown B-type"; break;
            }
            break;
        case JAL: str = "jal"; break;
        case JALR: str = "jalr"; break;
        case LUI: str = "lui"; break;
        case AUIPC: str = "auipc"; break;
        default: str="invalid instruction";
    }

    str = "\n" + str + " x" + to_string(rd) + ", x" + to_string(rs1);
    if (opcode == I) str += ", " + to_string(rs2);
    else str += ", x" + to_string(rs2);
    return str;
}