#include "mem.h"
using namespace std;

// const int mem::mem_size;

mem::mem() 
{
    size=0;
    startPC=0;
}

uint32_t mem::getMem(uint32_t addr){
    if (addr >= mem_size){
        cout << "invalid memory address" <<endl;
        return 0;
    }
    return sram[addr];
}

uint8_t mem::getMem_byte(uint32_t addr){
    if (addr >= mem_size){
        cout << "invalid memory address" << endl;
        return 0;
    }
    return (uint8_t) (sram[addr] & 0x000000ff);
}
void mem::setMem(uint32_t addr, uint32_t val){
    if (addr >= mem_size){
        cout << "invalid memory address" << endl;
        return;
    }
    sram[addr] = val;
}

int mem::getStartPC() {
    return startPC;
}

void mem::setStartPC(int index) {
    startPC = index;
}

int mem::getSize() {
    return size;
}

void mem::incrSize() {
    size += 1;
}