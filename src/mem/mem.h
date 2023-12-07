#include <iostream>
#include <cstdint>

#ifndef __MEM_H
#define __MEM_H

class mem {
    static const int mem_size = 2048;
    uint32_t sram[mem_size] = {0};
    int startPC; 
    int size;

public:
    mem();
    uint32_t getMem(uint32_t addr);
    uint8_t getMem_byte(uint32_t addr);
    void setMem(uint32_t addr, uint32_t val);
    int getStartPC(); //starting address
    void setStartPC(int);
    int getSize(); //number of instructions to read
    void incrSize();
};

#endif
