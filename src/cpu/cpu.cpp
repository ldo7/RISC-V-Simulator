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
    PC = 0;
    totalTime=0;
}

void cpu::run()
{

        // userInput();
    for (int i = 0; i < imem.getSize(); i++){
        int addr = PC/4;
        uint8_t opcode = getOpcode(imem.getMem(addr));
        switch (opcode)
        {
        case R:
            r_type(imem.getMem(addr));
            PC += 4;
            break;
        case I:
            i_type(imem.getMem(addr));
            PC += 4;
            break;
        case S:
            s_type(imem.getMem(addr));
            PC += 4;
            break;
        case L:
            l_type(imem.getMem(addr));
            PC += 4;
            break;
        case B:
            b_type(imem.getMem(addr));
            break;
        case JAL:
            jal(imem.getMem(addr));
            break;
        case JALR:
            jalr(imem.getMem(addr));
            break;
        case LUI:
            lui(imem.getMem(addr));
            PC += 4;
            break;
        case AUIPC:
            auipc(imem.getMem(addr));
            PC += 4;
            break;
        }
    }
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
        int16_t imm = getimm12(instr); 
        int8_t baseReg = getrs1(instr);
        int16_t sourceRegVal = getReg(sourceReg);
        int32_t shiftSourceVal = (sourceRegVal & 0b00000000000000000000000011111111);
        //>> 24;
        int8_t baseRegVal = getReg(baseReg);
        int32_t memAddr = alu.calculate(baseRegVal, imm, 0);
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
        int8_t memVal8 = dmem.getMem_byte(memAddr-dmem.getStartPC());
        cout << "Get start" << dmem.getStartPC() << endl;
        cout << "Dmem function: " << dmem.getMem_byte(memAddr-dmem.getStartPC());
        int32_t memVal;

    cout << "Base Address: " << baseAddr << endl;
    cout << "Immediate Value: " << imm << endl;
    cout << "Calculated Mem Address: " << memAddr << endl;
    cout << "Calculated Mem Value (memval8): " << memVal8 << endl;
        //check if signed and left most bit is 1 for negative
        //if(sign == 1  && (memVal8 & 0x80)){
        if((((memVal8 & 0b10000000) >> 7 ) == 1)&& sign == 0){
            memVal = 0xffffff00 | memVal8;
            } else{
                cout << "not negative" << endl;
                memVal = (uint32_t) memVal8;
            }
        
        cout << "Calculated Mem Value (memval): " << memVal << endl;
        reg.writeReg(rd, memVal);
        cout <<"result: " <<memVal<<endl;

    }
}
       /* int16_t sourceRegVal = getReg(sourceReg);
        int32_t memAddr = alu.calculate(sourceRegVal, bitShift, 0);
        int8_t memVal8 = dmem.getMem_byte(memAddr);
        int32_t memVal;

        // Print memory content before load operation
        cout << "Memory Content Before Load: ";
        for (int i = 0; i < 10; ++i) {
            cout << static_cast<int>(dmem.getMem_byte(i)) << " ";
        }
        cout << endl;

        // Print information about memory access
        cout << "Reading Memory at Address " << memAddr << ": " << static_cast<int>(memVal8) << endl;

        //check if signed and left most bit is 1 for negative
        if (sign == 1 && (memVal8 & 0x80))
        {
            // LB and negative
            memVal = (static_cast<int32_t>(memVal8) << 24) >> 24;
        }
        else
        {
            // LB or LBU
            memVal = static_cast<int32_t>(memVal8);
        }

        // Print information about the result
        cout << "result: " << static_cast<int>(memVal) << endl;

        // Print memory content after load operation
        cout << "Memory Content After Load: ";
        for (int i = 0; i < 10; ++i) {
            cout << static_cast<int>(dmem.getMem_byte(i)) << " ";
        }
        cout << endl;

        uint8_t rd = getrd(instr);
        reg.writeReg(rd, memVal);
        */
        
    

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
void cpu::word(uint32_t instr, uint8_t bitShift, int loadStore)
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
        dmem.setMem(memAddr, sourceRegVal);
        cout << "result raw: " << sourceRegVal<<endl;
        cout <<"result: " <<static_cast<int>(sourceRegVal)<<endl;

    }
    if(loadStore == 1){
        
        //check here whether signed or unsigned based on function argument param sign (only byte and halfword)
        uint8_t rd = getrd(instr);
        int16_t sourceRegVal = getReg(sourceReg);
        int32_t memAddr = alu.calculate(sourceRegVal, bitShift, 0);
        int32_t memVal = dmem.getMem(memAddr);
        reg.writeReg(rd, memVal);
        cout <<"result: " <<static_cast<int>(memVal)<<endl;
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
    cout << stringify(instr)<< endl;
     cout << "val1:" << static_cast<int>(val1) << " val2:" << static_cast<int>(val2) << " result:" << static_cast<int>(result) << endl;
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

    cout << stringify(instr) << endl;
    int32_t result = alu.calculate(val1, val2, alu_op); // execute
    reg.writeReg(rd, result);                           // write to reg

    // DEBUG
     cout << "val1:" << static_cast<int>(val1) << " val2:" << static_cast<int>(val2) << " result:" << static_cast<int>(result) << endl;
    // cout << "(binary)result:" << bitset<32>(result)<<endl;
    // cout << "register: "<<reg.readReg(rd)<<endl;   //check if stored properly
}
void cpu::s_type(uint32_t instr)
{
    cout << stringify(instr) << endl;
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
    cout << stringify(instr) << endl;
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
}
void cpu::l_type(uint32_t instr)
{
   cout << stringify(instr) << endl; 
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
            halfword(instr, storeBit, storeLoad, sign);
    }

    if(funct3 == 0b010){
            //pass to word w/ '1', key for load
            //word(instr, storeBit, storeLoad);
            word(instr, storeBit, storeLoad);
    }
}

void cpu::jal(uint32_t instr)
{
    cout << stringify(instr) << endl;
}
void cpu::jalr(uint32_t instr)
{
    cout << stringify(instr) << endl;
}
void cpu::lui(uint32_t instr)
{
    cout << stringify(instr) << endl;
    uint8_t rd = getrd(instr);
    uint32_t imm = getimm20(instr);
    uint32_t result = imm << 12;
    reg.writeReg(rd, result);
    cout << result;
}
void cpu::auipc(uint32_t instr)
{
    cout << stringify(instr) << endl;
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
void cpu::writeFile()
{
  string output = "test.asm";
  ofstream file(output, ios::app); // append
  if (!file)
  {
    cout << "ERROR. Cannot open file";
    return;
  }

  // get translated instruction from CPU.decode() ???
  // file << cpu.getAsmInstruction() << endl;
  file << "TESTING OUTPUT" << endl;
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

void cpu::userInput()
{
    // clock_t timer;
  uint32_t breakpoints[5] = {0xFFFFFFFF};
  int num;
  string input = " ";
  displayOptions();
  cout << "\n\nEnter a command: ";
  getline(cin, input);
  cout << endl << input;

  // Remove all whitespaces from input
//   input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());
//   string input;// = input.substr(0, 1);
//   cin >> c;

  // VALIDATE INPUT
  //  run - execute all
  //  c - continue
  if (input == "r" || input == "c")
  {
    // clockStart();
        for (int i = 0; i < imem.getSize(); i++){
        uint8_t opcode = getOpcode(imem.getMem(i));
        switch (opcode)
        {
        case R:
            r_type(imem.getMem(i));
            PC += 4;
            break;
        case I:
            i_type(imem.getMem(i));
            PC += 4;
            break;
        case S:
            s_type(imem.getMem(i));
            PC += 4;
            break;
        case L:
            l_type(imem.getMem(i));
            PC += 4;
            break;
        case B:
            b_type(imem.getMem(i));
            break;
        case JAL:
            jal(imem.getMem(i));
            break;
        case JALR:
            jalr(imem.getMem(i));
            break;
        case LUI:
            lui(imem.getMem(i));
            break;
        case AUIPC:
            auipc(imem.getMem(i));
            break;
        }
    }


  }
  // s - step through
  else if (input == "s")
  {
  }
  // x0 - x31
  else if (input == "x")
  {
    input = input.substr(1, 2);
    num = stoi(input);
    if (num > 31)
      cout << "Invalid input\n";
    // else{
    //     cout << CPU.getReg(num);
    // }
  }
  // 0x12345678 - return val at address
  else if (input == "0")
  {
    input = input.substr(2, 8);
    num = stoi(input);
    // cout << CPU.getMem(num);
  }
  // pc - return PC
  else if (input == "p")
  {
    if (input != "pc")
      cout << "Invalid input\n";
    // else{
    //     cout << "0x"<<CPU.getPC();
    // }
  }
  // insn - return NEXT asm instruction
  else if (input == "i")
  {
    if (input != "insn")
      cout << "Invalid input\n";
    // else{
    //     cout << CPU.debug();
    // }
  }
  // b[pc] - add breakpoint     assume format -> b[0x12345678]
  else if (input == "b")
  {
    input = input.substr(4, 8);
    num = stoi(input);
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
