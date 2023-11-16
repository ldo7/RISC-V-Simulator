#include <iostream>
#include <cstdint>

#ifndef __MEM_H
#define __MEM_H

class mem {
    const int static mem_size = 2048; 
    uint32_t sram[2048];
public:
    mem();
    uint32_t get_mem(uint32_t addr);
    uint8_t get_mem_byte(uint32_t addr);
    void set_mem(uint32_t addr, uint32_t val);
};

#endif
