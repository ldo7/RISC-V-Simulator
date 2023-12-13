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
    //const uint32_t mem_size2 = 1 << 32;  // 4 GB

    if (addr >= mem_size){
        std::cout << "Mem: invalid memory address" << std::endl;
        return 0;
    }
    cout << "Valid memory" << endl;
    cout << "Address: " << addr << endl;
    cout << "S ram address: " << sram[addr] << endl;
    uint8_t result = sram[addr] & 0xFF;
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