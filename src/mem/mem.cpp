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
    static const int mem_size = 1024; // Choose an appropriate size for your hash map
    vector<pair<uint32_t, uint32_t>> hashTable[mem_size]; // Each entry in the hash table is a vector of key-value pairs
    mem memory; // The mem class for memory operations

    // Hash function to map keys to indices
    int hash(uint32_t key) {
        return key % mem_size;
    }

public:
    HashMap() {
        // Initialize memory
        memory = mem();
    }

    void insert(uint32_t key, uint32_t value) {
        int index = hash(key);
        hashTable[index].push_back(make_pair(key, value));
    }

    uint32_t get(uint32_t key) {
        int index = hash(key);
        for (const auto &pair : hashTable[index]) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        cout << "Key not found" << endl;
        return 0; // Key not found
    }

    void remove(uint32_t key) {
        int index = hash(key);
        for (auto it = hashTable[index].begin(); it != hashTable[index].end(); ++it) {
            if (it->first == key) {
                hashTable[index].erase(it);
                return;
            }
        }
        cout << "Key not found" << endl;
    }
};

int main() {
    HashMap hashMap;

    hashMap.insert(1, 42);
    hashMap.insert(2, 55);

    cout << "Key 1: " << hashMap.get(1) << endl;
    cout << "Key 2: " << hashMap.get(2) << endl;

    hashMap.remove(1);
    cout << "Key 1 after removal: " << hashMap.get(1) << endl; // Key not found

    return 0;
}
