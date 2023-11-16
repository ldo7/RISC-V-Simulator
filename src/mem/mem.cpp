#include "mem.h"
using namespace std;

mem::mem() 
{
    for (int i = 0; i< mem_size; i++){
        sram[i] = 0;
    }
}

uint32_t mem::get_mem(uint32_t addr){
    if (addr >= mem_size){
        cout << "invalid memory address" <<endl;
        return 0;
    }
    return sram[addr];
}

uint8_t mem::get_mem_byte(uint32_t addr){
    if (addr >= mem_size){
        cout << "invalid memory address" << endl;
        return 0;
    }
    return (uint8_t) (sram[addr] & 0x000000ff);
}
void mem::set_mem(uint32_t addr, uint32_t val){
    if (addr >= mem_size){
        cout << "invalid memory address" << endl;
        return;
    }
    sram[addr] = val;
}
