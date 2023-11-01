// #include <iostream>
// #include "mem.h"
// using namespace std;

// mem::mem() 
// {
//     for (int i = 0; i< mem_size; i++){
//         sram[i] = 0;
//     }
// }

// uint32_t mem::get_mem(uint32_t addr){
//     if (addr >= mem_size){
//         cout << "invalid memory address" <<endl;
//         return 0;
//     }
//     return sram[addr];
// }

// uint8_t mem::get_mem_byte(uint32_t addr){
//     if (addr >= mem_size){
//         cout << "invalid memory address" << endl;
//         return 0;
//     }
//     return (uint8_t) (sram[addr] & 0x000000ff);
// }
// void mem::set_mem(uint32_t addr, uint32_t val){
//     if (addr >= mem_size){
//         cout << "invalid memory address" << endl;
//         return;
//     }
//     sram[addr] = val;
// }

#include <iostream>
#include <vector>
#include "mem.h"

using namespace std;

class HashMap {
    
    private:
    static const int mem_size = 1024; //size for your hash map
    vector<pair<uint32_t, uint32_t>> hashTable[mem_size]; // hashtable store key-value/ Each entry is a vector of key-value pairs
    mem memory; // The mem class for memory operations

    int hash(uint32_t key) {
        return key % mem_size;
    }
    
    public:
    HashMap() {
        // Initialize memory
        memory = mem();
    }

