#include <iostream>
#include <fstream>
#include <bitset>
#include <ctime>
#include <vector>
#include <unistd.h>
#include "cpu/cpu.h"
using namespace std;

void readFile();
void writeFile();
void displayOptions();
void userInput();
void clockStart();
void clockStop();
clock_t timer;
int totalTime = 0;
uint32_t breakpoints[5] = {0xFFFFFFFF}; // initialize to FF (never reached)

int main()
{
    readFile();

    // INTANTIATE & RUN CPU
    cpu cpu;
    cpu.run();

    // REQUEST USER INPUT
    while (true)
    {
        userInput();
    }

    // DISPLAY TIME
    cout << "\nTotal Time:" << totalTime;
    clockStart();
    sleep(10);
    clockStop();
    cout << fixed << "\nTotal Time:" << (float)totalTime / CLOCKS_PER_SEC << " sec" << scientific;

    writeFile();

    return 0;
}

// Parse .DAT file
void readFile()
{
    // OPEN file
    string line, input = "input.dat";
    ifstream file(input);
    vector<uint32_t> list;
    if (!file)
    {
        cout << "ERROR. Cannot open file";
        return;
    }
    cout << "RISCV SIMULATOR\n"
         << "\n~~ Loading instructions ~~\n";

    // READ file
    bitset<32> combinedBinary(0); // intialize 32-bit value = 0000 0000 0000 0000
    string combinedString;
    while (getline(file, line))
    {
        uint32_t val = 0;
        // val = bitset<32>(line).to_ulong(); // Convert the binary string to uint32_t

        // every 4 lines = 1 instruction
        combinedString = line + combinedString;
        if (combinedString.length() == 32) // append line until 32-bits
        {
            // convert string to binary
            for (int i = 0; i < 32; i++)
            {
                if (combinedString[i] == '1')
                    combinedBinary.set(i);
                else
                    combinedBinary.reset(i);
            }
            // insert the value into the vector
            list.push_back(combinedBinary.to_ulong());
            // cout << combinedBinary<<"\n";  //DEBUG, check little-endian order
            combinedBinary.reset();
            combinedString.clear();
        }
    }
    file.close();

    // for(bitset<32> instr:list){  //DEBUG
    //     cout << instr << "\n";
    // }

    cout << "~~ Loading complete ~~\n";
}

// Write to .ASM file for Debugging
void writeFile()
{
    string output = "output.asm";
    ofstream file(output, ios::app); // append
    if (!file)
    {
        cout << "ERROR. Cannot open file";
        return;
    }

    // get translated instruction from CPU.decode() ???
    file << "TESTING OUTPUT" << endl;
    file.close();
}

void clockStart()
{
    timer = clock();
}

void clockStop()
{
    timer = clock() - timer;
    totalTime += timer;
}

void displayOptions()
{
    cout << "\n"
         << "r     - run entire program"
         << "\n"
         << "s     - run next instruction"
         << "\n"
         << "x0    - view content in this register"
         << "\n"
         << "0x12345678 - view content at this address"
         << "\n"
         << "pc    - viewb  PC value"
         << "\n"
         << "insn  - view next instruction"
         << "\n"
         << "b[pc] - set breakpoint"
         << "\n"
         << "c     - continue execution";
}

void userInput()
{
    string input = " ";
    displayOptions();
    cout << "\n\nEnter a command: ";
    getline(cin, input);

    // Remove all whitespaces from input
    input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());
    string c = input.substr(0, 1);
    int num;

    // VALIDATE INPUT
    //  run - execute all
    //  c - continue
    if (c == "r" || c == "c")
    {
        // clockStart();
    }
    // s - step through
    else if (c == "s")
    {
    }
    // x0 - x31
    else if (c == "x")
    {
        c = input.substr(1, 2);
        num = stoi(c);
        if (num > 31)
            cout << "Invalid input\n";
        // else{
        //     cout << CPU.getReg(num);
        // }
    }
    // 0x12345678 - return val at address
    else if (c == "0")
    {
        c = input.substr(2, 8);
        num = stoi(c);
        // cout << CPU.getMem(num);
    }
    // pc - return PC
    else if (c == "p")
    {
        if (input != "pc")
            cout << "Invalid input\n";
        // else{
        //     cout << "0x"<<CPU.getPC();
        // }
    }
    // insn - return NEXT asm instruction
    else if (c == "i")
    {
        if (input != "insn")
            cout << "Invalid input\n";
        // else{
        //     cout << CPU.debug();
        // }
    }
    // b[pc] - add breakpoint     assume format -> b[0x12345678]
    else if (c == "b")
    {
        c = input.substr(4, 8);
        num = stoi(c);
        // ASSUME SORTED IN-ORDER....
        for (int i = 0; i < 5; i++)
        { // check is breakpoints[] is full
            if (breakpoints[i] != 0xFFFFFFFF)
            {
                breakpoints[i] = num;
                break;
            }
        }
    }
    else
    {
        cout << "Invalid input\n";
    }
}
