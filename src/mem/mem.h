#include <iostream>
#include <cstdint>
<<<<<<< HEAD
// #include <vector>

#ifndef RISCV_SIMULATOR_MEM_H
#define RISCV_SIMULATOR_MEM_H

class mem {
    const int mem_size = 2048;  //change to hashmap
=======

#ifndef __MEM_H
#define __MEM_H

class mem {
    const int static mem_size = 2048; 
>>>>>>> origin/main
    uint32_t sram[2048];
public:
    mem();
    uint32_t get_mem(uint32_t addr);
    uint8_t get_mem_byte(uint32_t addr);
    void set_mem(uint32_t addr, uint32_t val);
};

#endif
