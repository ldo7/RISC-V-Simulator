#include <iostream>
#include <cstdint>
#include <stdint.h>
#include <string>
#include "../mem/mem.cpp"
#include "../reg/reg.cpp"
#include "../alu/alu.cpp"

#ifndef __CPU_H__
#define __CPU_H__

class cpu
{
public:  // functions called in Main
    cpu();
    void run();
    uint32_t getPC();
    uint32_t getReg(uint8_t index);
    uint32_t getMem(uint32_t addr);

private:
    int num;
    void privado();
    mem mem;
    reg reg;
    alu alu;
    uint32_t PC;
    // string AsmInstruction;
    // string setAsmInstruction();
    uint8_t getOpcode(uint32_t instr);
    uint8_t getrd(uint32_t instr);
    uint8_t getrs1(uint32_t instr);
    uint8_t getrs2(uint32_t instr);
    uint8_t getfunct3(uint32_t instr);
    uint8_t getfunct7(uint32_t instr);
    uint32_t getimm12(uint32_t instr);
    uint32_t get_branch_imm(uint32_t instr);
    uint32_t get_jal_offset(uint32_t instr);
};

#endif