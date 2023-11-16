#include "cpu.h"

cpu::cpu()
{
    PC = 0;
    AsmInstruction = "";
}

void cpu::run()
{
    // parse opcode

    // switch case for instruction type

    //convert to String
}

//USER OPTIONS FUNCTIONS
uint32_t cpu::getPC()
{
    return PC;
}
uint32_t cpu::getReg(uint8_t index)
{
    return regs.readReg(index);
}
uint32_t cpu::getMem(uint32_t addr)
{
    return mem.get_mem(addr);
}
string cpu::getAsmInstruction(uint32_t instr){

}

// DECODE FUNCTIONS
uint8_t cpu::getOpcode(uint32_t instr)
{
    return (uint8_t)instr & 0x01111111;
}

uint8_t cpu::getrd(uint32_t instr)
{
    return (uint8_t)(instr >> 7) & 0x00011111;
}

uint8_t cpu::getrs1(uint32_t instr)
{
    return (uint8_t)(instr >> 15) & 0x00011111;
}

uint8_t cpu::getrs2(uint32_t instr)
{
    return (uint8_t)(instr >> 20) & 0x00011111;
}

uint8_t cpu::getfunct3(uint32_t instr)
{
    return (uint8_t)(instr >> 12) & 0x00000111;
}

uint8_t cpu::getfunct7(uint32_t instr)
{
    return (uint8_t)(instr >> 25) & 0x01111111;
}

 uint8_t cpu::getALU_op(uint32_t instr)
 {
    uint8_t funct3 = getfunct3(instr);
    uint8_t funct7 = getfunct7(instr);
    if (funct7 == 0x0100000){
        return funct3 | funct7;
    } else {
        return funct3;
    }
 }

//R-TYPE EXECUTE
#include "cpu.h"

void cpu::r_type(uint32_t instr)
{
    uint8_t alu_op = getALU_op(instr);
    uint8_t rd = getrd(instr);
    uint8_t rs1 = getrs1(instr);
    uint8_t rs2 = (uint8_t)(instr >> 20) & 0x00011111;

    //read data from registers
    uint32_t in1 = regs.readReg(rs1);
    uint32_t in2 = regs.readReg(rs2);

    //r-type execute
    uint32_t result = ALU.calculate(in1, in2, alu_op);

    //write result to register
    regs.writeReg(rd, result);
}
