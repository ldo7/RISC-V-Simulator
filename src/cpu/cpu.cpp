#include <iostream>
#include "cpu.h"
using namespace std;

bool debug = true;
uint8_t x=0;

cpu::cpu(mem imem,mem dmem)
{
    this->imem = imem;
    this->dmem = dmem;
    running = true;
    noBreakpt = true;
    PC = 0;
    totalTime=0;
}

void cpu::runInstruction(){
    int addr = PC/4;
    uint32_t instr = imem.getMem(addr);
    string str = stringify(instr);// << endl;
    cout << str;// << endl;
    // writeFile(str);
    uint8_t opcode = getOpcode(instr);
    switch(opcode) {
        case R: r_type(instr); PC += 4; break;
        case I: i_type(instr); PC += 4; break;
        case S: s_type(instr); PC += 4; break;
        case L: l_type(instr); PC += 4; break;
        case B: b_type(instr); break;
        case JAL: jal(instr); break;
        case JALR: jalr(instr); break;
        case LUI: lui(instr); PC += 4; break;
        case AUIPC: auipc(instr); PC += 4; break;
    }
}

bool cpu::checkBreakpt(){
    for (int i = 0; i < 5; i++)
    {
        if(breakpoints[i]==PC)
            return false;
    }
    return true;
}

void cpu::run()
{
    string input, bk;
    char ch;
    int num;
    uint32_t instr;

//Debug instructions w/out User Options - just 'r'
// while(keepGoing()){
//     runInstruction();
// }

    while(keepGoing()){
        // checkBreakpt();
        input = userInput();
        ch = input[0];
        switch(ch){
            case 'r':
                clockStart();
                while(noBreakpt && keepGoing()){
                    runInstruction();
                    noBreakpt=checkBreakpt();
                }
                clockStop();
                break;
            case 'c':
                do{
                    clockStart();
                    runInstruction();
                    noBreakpt=checkBreakpt();
                }
                while(noBreakpt && keepGoing());
                clockStop();
                break;
            case 's':
                // clockStart();
                runInstruction();
                // clockStop();
                break;
            case 'x':
                input = input.substr(1, 2);
                num = stoi(input);
                cout << "x" << num << " = " << reg.readReg(num) <<endl;
                break;
            case '0':
                input = input.substr(2, 8);
                num = stoi(input);
                if (num < 0x10010000) //imem
                    cout << "0x" << num << " = " << imem.getMem(num) << endl;
                else if (num > 0x10010000 && num < 0x7fffffff)
                    cout << "0x" << num << " = " << dmem.getMem(num-dmem.getStartPC()) << endl;
                break;
            case 'p':
                cout << "PC = " << PC << endl;
                break;
            case 'i':
                num = PC/4+4;
                instr = imem.getMem(num);
                cout << "next instruction: " << stringify(instr) << endl;
                break;
            case 'b':
                //b[10] b[1]
                // getline(cin,bk,']');
                // cout << bk << endl;
                bk = input.substr(2,1+bk.length());
                num = stoi(bk);
                if (num % 4 != 0) {
                    cout << "invalid address" << endl;
                    break;
                }
                
                for (int i = 0; i < 5; i++)
                {
                    if(breakpoints[i]!=0) {
                        breakpoints[i]=num;
                        break;
                    } //assume no breakpt in 1st line
                }
                break;
            default:
                cout << "invalid input" << endl;
        }

        keepGoing(); //check if end of program
    }

    //Display all register at the end
    const int columnWidth = 10;
    cout << endl << setfill('-') << setw(columnWidth * 2) << "" << setfill(' ') << endl;
    cout << left << setw(columnWidth) << "Register" << setw(columnWidth) << "Value" << endl;
    cout << setfill('-') << setw(columnWidth * 2) << "" << setfill(' ') << endl;
    for (int i = 0; i < 32; i++) 
        cout << left << setw(columnWidth) << "x" + to_string(i) << setw(columnWidth) << reg.readReg(i) << endl;
    cout << endl << "--------------------" << endl;
    cout << " Total time: " << totalTime/1000.0 << "s" 
         << endl <<"--------------------" << endl << endl;
}

// USER OPTIONS FUNCTIONS
uint32_t cpu::getPC()
{
    return PC;
}
uint32_t cpu::getReg(uint8_t index)
{
    return reg.readReg(index);
}
uint32_t cpu::getMem(uint32_t addr)
{
    return dmem.getMem(addr);
}

// DECODE FUNCTIONS
uint8_t cpu::getOpcode(uint32_t instr)
{
    return (uint8_t)instr & 0b01111111;
}
uint8_t cpu::getrd(uint32_t instr)
{
    return (uint8_t)(instr >> 7) & 0b00011111;
}
uint8_t cpu::getrs1(uint32_t instr)
{
    return (uint8_t)(instr >> 15) & 0b00011111;
}
uint8_t cpu::getrs2(uint32_t instr)
{
    return (uint8_t)(instr >> 20) & 0b00011111;
}
uint8_t cpu::getfunct3(uint32_t instr)
{
    return (uint8_t)(instr >> 12) & 0b00000111;
}
uint8_t cpu::getfunct7(uint32_t instr)
{
    return (uint8_t)(instr >> 25) & 0b01111111;
}
int16_t cpu::getimm12(uint32_t instr)
{
    int16_t val = (int16_t)(instr >> 20) & 0b0111111111111;
    if (val & 0b100000000000)               // check msb
        return (val | 0b1111000000000000);  // if (-), sign-extend
    return val;
}
int32_t cpu::getimm20(uint32_t instr)
{
    return (instr >> 12);
}
uint8_t cpu::getALU_op(uint32_t instr)
{
    uint8_t funct3 = getfunct3(instr);
    uint8_t funct7 = getfunct7(instr);

    if (funct7 == 0b0100000)
        return funct3 | funct7;
    return funct3;
}
uint32_t cpu::get_branch_imm(uint32_t instr)
{
    uint32_t imm = 0;
    uint32_t imm_0 = 0;
    uint32_t imm_11 = (instr >> 7) & 0b0000001;
    uint32_t imm_4_1 = (instr >> 8) & 0b00001111;
    uint32_t imm_10_5 = (instr >> 25) & 0b0000000111111;
    uint32_t imm_12 = (instr >> 31) & 0b0000000001;

    // Combine the extracted bits to get the 13-bit immediate
    // imm =  imm_11 | imm_4_1 | imm_10_5 | imm_12;
    imm = imm_11  | (imm_4_1 << 1) | (imm_10_5 << 5) | (imm_12 << 11);
  
    return imm;
}
int16_t cpu::get_jal_offset(uint32_t instr)
{
    uint32_t offset = 0;
    uint32_t offset_0 = 0;
    uint32_t offset20 = (instr & 0x80000000) >> 11;
    uint32_t offset10_1 = (instr & 0b01111111111000000000000000000000) >> 20;
    uint32_t offset11 =    (instr & 0b00000000000100000000000000000000) >> 9;
    uint32_t offset19_12= (instr & 0b00000000000011111111000000000000);

    // Combine the extracted bits to get the 21-bit offset
    offset = offset20 | offset19_12 | offset11 | offset10_1 | offset_0;

    // add sign extension if the offset is negative
    if ((offset20 >> 20) != 0){
        offset = offset | 0xfff00000; 
    }
    return (uint16_t)offset;
}

void cpu::byte(uint32_t instr, uint16_t bitShift, int loadStore, int sign)
{
    uint8_t sourceReg = getrs2(instr);

    if (loadStore == 0)
    {
        int8_t baseReg = getrs1(instr);
        int8_t sourceRegVal = getReg(sourceReg);
        int32_t shiftSourceVal = sourceRegVal >> 24;
        int8_t baseRegVal = getReg(baseReg);
        int32_t memAddr = alu.calculate(baseRegVal, bitShift, 0);
        imem.setMem(memAddr, shiftSourceVal);
        cout <<"result: " <<shiftSourceVal<<endl;
    }
if(loadStore == 1){
        //check here whether signed or unsigned based on function argument param sign (only byte and halfword)
        int8_t baseReg = getrs1(instr);
        int8_t rd = getrd(instr);
        int32_t imm = getimm12(instr);
        int32_t baseAddr = getReg(baseReg);
        int32_t memAddr = alu.calculate(baseAddr, imm, 0);
        cout << "Mem Address: " << memAddr << endl;
        int16_t memVal8 = dmem.getMem_byte(memAddr);
        int32_t memVal;

    cout << "Base Address: " << baseAddr << endl;
    cout << "Immediate Value: " << imm << endl;
    cout << "Calculated Mem Address: " << memAddr << endl;
        //check if signed and left most bit is 1 for negative
        //if(sign == 1  && (memVal8 & 0x80)){
        if(((memVal8 & 0b10000000) >> 7 ) == 1){
            memVal = 0xffffff00 | memVal8;
            } else{
                cout << "not negative" << endl;
                memVal = (uint32_t) memVal8;
            }
        cout << "Calculated Mem Value (memval8): " << memVal8 << endl;
        cout << "Calculated Mem Value (memval): " << memVal << endl;
        reg.writeReg(rd, memVal);
        cout <<"result: " <<memVal<<endl;

    }
}
void cpu::halfword(uint32_t instr, uint16_t bitShift, int loadStore, int sign)
{
    uint8_t sourceReg = getrs2(instr);

    if (loadStore == 0)
    {
        int8_t baseReg = getrs1(instr);
        int8_t sourceRegVal = getReg(sourceReg);
        int32_t shiftSourceVal = sourceRegVal >> 16;
        int8_t baseRegVal = getReg(baseReg);
        int32_t memAddr = alu.calculate(baseRegVal, bitShift, 0);
        imem.setMem(memAddr, shiftSourceVal);
        cout <<"result: " <<shiftSourceVal<<endl;

    }
    if (loadStore == 1)
    {
        // check here whether signed or unsigned based on function argument param sign (only byte and halfword)
        int8_t sourceRegVal = getReg(sourceReg);
        int32_t memAddr = alu.calculate(sourceRegVal, bitShift, 0);
        int16_t memVal16 = dmem.getMem(memAddr);
        int32_t memVal;
        //uint32_t memVal = memAddr << 16;
        //uint8_t rd = getrd(instr);
        //reg.writeReg(rd, memVal);

        //lh signed
        //check if negative (if 16th bit is 1, negative)
        //if not negative, just cast to 32 bit
        if(sign == 1 && (memVal16 & 0x8000)){
            memVal = (uint32_t)(uint32_t (memAddr) << 16);
        }
        else{
            memVal = memAddr << 16;
        }
        uint8_t rd = getrd(instr);
        reg.writeReg(rd, memVal);
        cout <<"result: " <<static_cast<int>(memVal)<<endl;

    
    }
}
void cpu::word(uint32_t instr, uint8_t bitShift, int loadStore, int sign)
{
    // check if string
    // if string,
    uint8_t sourceReg = getrs2(instr);

    if (loadStore == 0)
    {
        // store word
        int16_t baseReg = getrs1(instr);
        int32_t sourceRegVal = getReg(sourceReg);
        int16_t baseRegVal = getReg(baseReg);
        int32_t memAddr = alu.calculate(baseRegVal, bitShift, 0);
        dmem.setMem(memAddr/4, sourceRegVal);
        cout << endl<<"result raw: " << sourceRegVal<<endl;
        cout <<"result: " <<static_cast<int>(sourceRegVal)<<endl;

    }
    if(loadStore == 1){
        
        //check here whether signed or unsigned based on function argument param sign (only byte and halfword)
        uint8_t rd = getrd(instr);
        int16_t sourceRegVal = getReg(sourceReg);
        int32_t memAddr = alu.calculate(sourceRegVal, bitShift, 0);
        int32_t memVal = dmem.getMem(memAddr/4);
        reg.writeReg(rd, memVal);
        cout <<endl<<"result: " <<static_cast<int>(memVal)<<endl;
    }
}

void cpu::r_type(uint32_t instr)
{
    uint8_t alu_op = getALU_op(instr);
    uint8_t rd = getrd(instr);
    uint8_t rs1 = getrs1(instr);
    int8_t rs2 = getrs2(instr);

    uint32_t val1 = reg.readReg(rs1);                   // read from reg
    uint32_t val2 = reg.readReg(rs2);
    uint32_t result = alu.calculate(val1, val2, alu_op); // execute
    reg.writeReg(rd, result);                            // write to reg

    // DEBUG
     cout << endl<<"val1:" << static_cast<int>(val1) << " val2:" << static_cast<int>(val2) << " result:" << static_cast<int>(result) << endl;
}
void cpu::i_type(uint32_t instr)
{
    uint8_t alu_op = getALU_op(instr);
    uint8_t rd = getrd(instr);
    uint8_t rs1 = getrs1(instr);
    int8_t rs2 = getrs2(instr); // only for shift instructions

    int32_t val1 = reg.readReg(rs1); // read from reg
    int32_t val2 = getimm12(instr);
    if (alu_op == SLL || alu_op == SRL || alu_op == SRA)
        val2 = rs2; // getShamt()

    int32_t result = alu.calculate(val1, val2, alu_op); // execute
    reg.writeReg(rd, result);                           // write to reg

    // DEBUG
     cout << endl<<"val1:" << static_cast<int>(val1) << " val2:" << static_cast<int>(val2) << " result:" << static_cast<int>(result) << endl;
    // cout << "(binary)result:" << bitset<32>(result)<<endl;
    // cout << "register: "<<reg.readReg(rd)<<endl;   //check if stored properly
}
void cpu::s_type(uint32_t instr)
{
   //following S-format
    // imm[11:5] || rs2 || rs1 || function3 || imm[4:0] || opcode
    // 7 bits || 5 bits || 5 bits || 3 bits || 5 bits || 7 bits
    int storeLoad = 0;
    uint8_t rd = getrd(instr);
    uint8_t rs1 = getrs1(instr);
    int8_t rs2 = getrs2(instr);

    uint8_t funct3 = getfunct3(instr);
    uint8_t funct7 = getfunct7(instr);

    //get 16 bit per RISC-V architecture by shifting funct 7 by 5 bits and combine with rd
    uint16_t storeBit = (((uint16_t) funct7) << 5) | (uint16_t) rd;
    //pass destination register or function7
    //function 3, (same as load) determines size

    //pass instruction, immediate, and loadStore value
    //loadStore will be 0 for store and 1 for load
    //and can pass opcode (however, opcode should always be 0 for load and store)
    
    switch(funct3){
        case 0b000:
            //pass to byte function w/ '0', key for store
            byte(instr, storeBit, storeLoad);
            break;
        case 0b001:
            //pass to halfword function w/ '0', key for store
            halfword(instr, storeBit, storeLoad);
            break;
        case 0b010:
            //pass to word function w/ '0', key for store
            word(instr, storeBit, storeLoad);
            break;
        default:
            //not store byte, halfword, or word
            break;

    }
}
void cpu::b_type(uint32_t instr)
{
    uint8_t funct3 = getfunct3(instr);
    uint32_t offset = get_branch_imm(instr);
    uint8_t rs1 = getrs1(instr);
    uint8_t rs2 = getrs2(instr);
    
    // Execute the branch instruction //
    // B-typed
    
    bool branch_taken = false;
    switch (funct3) {
        case 0b000: // BEQ
            branch_taken = (reg.readReg(rs1) == reg.readReg(rs2));
            break;
        case 0b001: // BNE
            branch_taken = (reg.readReg(rs1) != reg.readReg(rs2));
            break;
        case 0b100: // BLT
            branch_taken = (static_cast<int32_t>(reg.readReg(rs1)) < static_cast<int32_t>(reg.readReg(rs2)));
            break;
        case 0b101: // BGE
            branch_taken = (static_cast<int32_t>(reg.readReg(rs1)) >= static_cast<int32_t>(reg.readReg(rs2)));
            break;
        case 0b110: // BLTU
            branch_taken = (reg.readReg(rs1) < reg.readReg(rs2));
            break;
        case 0b111: // BGEU
            branch_taken = (reg.readReg(rs1) >= reg.readReg(rs2));
            break;
        default:
            // Handle unsupported funct3 values
            break;
    }

    // Using program counter to jump branch result
    if (branch_taken) {
        PC += offset;
    } else {
        PC += 4; // Jump to next instruction
    }
    
    //console output
    cout << endl;
}
void cpu::l_type(uint32_t instr)
{
 //following L-format
    // imm[11:0] || rs1 || function3 || rd || opcode
    // 12 bits || 5 bits || 3 bits || 5 bits || 7 bits
    int storeLoad = 1;
    uint8_t rd = getrd(instr);
    uint8_t rs1 = getrs1(instr);
    int8_t rs2 = getrs2(instr);
    //changed here
    uint8_t storeBit = getimm12(instr);

    uint8_t funct3 = getfunct3(instr);
    uint8_t funct7 = getfunct7(instr);
    //getimm12

    if(funct3 == 0b000 || funct3 == 0b100){
            //pass to byte function w/ '1', key for load
            //byte(instr, storeBit, storeLoad);
            int sign = 1;
            if(funct3 == 0b100){
                sign = 0;
            }
            byte(instr, storeBit, storeLoad, sign);
    }
    if(funct3 == 0b001 || funct3 == 0b101){
            //pass to halfword function w/ '1', key for load
            //halfword(instr, storeBit, storeLoad);
            int sign = 1;
            if(funct3 == 0b101){
                sign = 0;
            }
            halfword(instr, storeBit, storeLoad);
    }

    if(funct3 == 0b010){
            //pass to word w/ '1', key for load
            //word(instr, storeBit, storeLoad);
            word(instr, storeBit, storeLoad);
    }
}


void cpu::jal(uint32_t instr)
{
    uint8_t rd = getrd(instr);
    uint32_t offset = get_jal_offset(instr);
    uint32_t result = PC + offset;
    reg.writeReg(rd, result);
    PC += offset;
}
void cpu::jalr(uint32_t instr)
{
    uint32_t imm = getimm12(instr);
    imm = imm & 0xfffffffe; // set the least-significant bit of the result to zero
    if ((imm >> 12) != 0){
        imm = imm | 0xfffff000; // add sign extension
    }
    uint32_t PC = getPC();
    uint8_t rd = getrd(instr);
    uint8_t rs1 = getrs1(instr);
    uint32_t rs1_val = reg.readReg(rs1);
    uint32_t offset = rs1_val + imm;
    
    uint32_t result = PC + 4;
    reg.writeReg(rd, result);
    PC += offset;
}
void cpu::lui(uint32_t instr)
{
    uint8_t rd = getrd(instr);
    uint32_t imm = getimm20(instr);
    uint32_t result = imm << 12;
    reg.writeReg(rd, result);
    cout << " result:" << result << endl;
}
void cpu::auipc(uint32_t instr)
{
    uint8_t rd = getrd(instr);
    uint32_t imm = getimm20(instr);
    uint32_t result = PC + (imm << 12);
    reg.writeReg(rd, result);
    cout << " result:" << result << endl;
}

//convert binary to asm string representation
string cpu::stringify(int32_t instr)
{
    string str="";
    uint8_t opcode = getOpcode(instr);
    uint8_t aluOp = getALU_op(instr);
    uint8_t funct3 = getfunct3(instr);
    uint8_t funct7 = getfunct7(instr);
    uint16_t imm12 = getimm12(instr);
    uint16_t _jal = get_jal_offset(instr);
    uint32_t imm20 = getimm20(instr);
    uint8_t rs1 = getrs1(instr);
    uint8_t rs2 = getrs2(instr);
    uint8_t rd = getrd(instr);
    uint32_t imm_branch = get_branch_imm(instr);

    
    switch(opcode){
        case R:
        case I:
            switch (aluOp) {
                case ADD: str = "add"; break;
                case SUB: str = "sub"; break;
                case XOR: str = "xor"; break;
                case OR: str = "or"; break;
                case AND: str = "and"; break;
                case SLL: str = "sll"; break;
                case SRL: str = "srl"; break;
                case SRA: str = "sra"; break;
                case SLT: str = "slt"; break;
                case SLTU: str = "sltu"; break;
                default: str = "invalid aluOp"; 
            }
            if (opcode == I) str+="i";
            break;
        case L:
            switch (funct3) {
                case 0b000: str = "lb"; break;
                case 0b001: str = "lh"; break;
                case 0b010: str = "lw"; break;
                case 0b100: str = "lbu"; break;
                case 0b101: str = "lhu"; break;
                default: str = "invalid L-type"; break;
            }
            break;
        case S:
            switch (funct3) {
                case 0b000: str = "sb"; break;
                case 0b001: str = "sh"; break;
                case 0b010: str = "sw"; break;
                default: str = "unknown S-type"; break;
            }
            break;
        case B:
            switch (funct3) {
                case 0b000: str = "beq"; break;
                case 0b001: str = "bne"; break;
                case 0b100: str = "blt"; break;
                case 0b101: str = "bge"; break;
                case 0b110: str = "bltu"; break;
                case 0b111: str = "bgeu"; break;
                default: str = "unknown B-type"; break;
            }
            break;
        case JAL: str = "jal"; break;
        case JALR: str = "jalr"; break;
        case LUI: str = "lui"; break;
        case AUIPC: str = "auipc"; break;
        default: str="invalid instruction";
    }

    switch(opcode){
        case R:
        case I:
            str = "\n" + str + " x" + to_string(rd) + ", x" + to_string(rs1);
            if (opcode == I) str += ", " + to_string(imm12);
            else str += ", x" + to_string(rs2);
            break;
        case B: 
            str = "\n" + str + " x" + to_string(rs1) + ", x" + to_string(rs2) + ", label_" + to_string(imm_branch);
            break;
        case L:
            str = "\n" + str + " x" + to_string(rd) + ", " + to_string(imm12) +"(x" + to_string(rs1) +")";
            break;
        case S: 
            str = "\n" + str + " x" + to_string(rs2) + ", " + to_string(imm_branch) +"(x" + to_string(rs1) +")";
            break;
        case JALR:
            str = "\n" + str + " x" + to_string(rd) + ", x" + to_string(rs1) + ", " + to_string(imm12) ;
            break;
        case JAL:
            str = "\n" + str + " x" + to_string(rd) +", label_" + to_string(_jal);
            break;
        case AUIPC:
            str = "\n" + str + " x" + to_string(rd) +", label_" + to_string(imm20);
            break;
        case LUI:
            str = "\n" + str + " x" + to_string(rd) + ", " + to_string(imm20);
            break;
    }
    return str;
}



//RUN COMMANDS

// Write to .ASM file for Debugging
void cpu::writeFile(string str)
{
  string output = "output.asm";
  ofstream file(output, ios::app); // append
  if (!file)
  {
    cout << "ERROR. Cannot open file";
    return;
  }
    file << str << endl;
  file.close();
}

void cpu::clockStart()
{
  timer = clock();
}

void cpu::clockStop()
{
  timer = clock() - timer;
  totalTime += timer;
}

void cpu::displayOptions()
{
  cout << endl
       << "-----------------------------------------------"
       << endl
       << "r     - run entire program"
       << endl
       << "s     - run next instruction"
       << endl
       << "x0    - view content in this register"
       << endl
       << "0x12345678 - view content at this address"
       << endl
       << "pc    - view  PC value"
       << endl
       << "insn  - view next instruction"
       << endl
       << "b[pc] - set breakpoint"
       << endl
       << "c     - continue execution";
}

string cpu::userInput()
{
    // clock_t timer;
//   uint32_t breakpoints[5] = {0xFFFFFFFF};
  int num;
  string input = "";
  displayOptions();
  cout << endl << endl << "Enter a command: ";
  getline(cin, input);
  return input;

  // Remove all whitespaces from input
//   input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());
//   string input;// = input.substr(0, 1);
//   cin >> c;

  // VALIDATE INPUT
  //  run - execute all
  //  c - continue
//   if (input == "r" || input == "c")
//   {
//       // clockStart();

//   }
//   // s - step through
//   else if (input == "s")
//   {
//   }
//   // x0 - x31
//   else if (input == "x")
//   {
//       input = input.substr(1, 2);
//       num = stoi(input);
//     //   if (num > 31)
//     //       cout << "Invalid input\n";
//         //   cout << reg.readReg(num);
//   }
//   // 0x12345678 - return val at address
//   else if (input == "0")
//   {
//       input = input.substr(2, 8);
//       num = stoi(input);
//       // cout << CPU.getMem(num);
//       //assume address = multiple of 4
//       //return data memory or instruction? --> account for both b/c 0 - 0x10, 0x10 - 0x7f

//   }
//   // pc - return PC
//   else if (input == "p")
//   {
//       if (input != "pc")
//           cout << "Invalid input\n";
//       // else{
//       //     cout << "0x"<<CPU.getPC();
//       // }
//   }
//   // insn - return NEXT asm instruction
//   else if (input == "i")
//   {
//       if (input != "insn")
//           cout << "Invalid input\n";
//       // else{
//       //     cout << CPU.debug();
//       // }
//   }
//   // b[pc] - add breakpoint     assume format -> b[0x12345678]
//   else if (input == "b")
//   {
//       input = input.substr(4, 8);
//       num = stoi(input);
//       // ASSUME SORTED IN-ORDER....
//       for (int i = 0; i < 5; i++)
//       { // check is breakpoints[] is full
//           if (breakpoints[i] != 0xFFFFFFFF)
//           {
//               breakpoints[i] = num;
//               break;
//           }
//       }
//   }
//   else
//   {
//       cout << "Invalid input\n";
//   }
}

bool cpu::keepGoing(){
    // cout << "size"<<imem.getSize() << endl;
    return (PC/4) < imem.getSize(); //-1 to account for blank-zero instr
}
