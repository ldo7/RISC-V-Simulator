#include <iostream>
#include <fstream>
#include <bitset>
#include <ctime>
#include <vector>
#include <unistd.h>
#include "../cpu/cpu.cpp"
using namespace std;

vector<uint32_t> readFile();
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
  vector<uint32_t> instructions = readFile(); // LOAD INSTRUCTIONS 
  cpu cpu(instructions);                      // INTANTIATE & RUN CPU
  cpu.run();
  // bool cpu_is_running = true;
  // while (true)
  // {
  // userInput();  // REQUEST USER INPUT
  // clockStart();
  // cpu.fetch();
  // writeFile();
  // cpu.execute();
  // clockStop();
  // if (cpu.getPC() == 0) //check if end
  //     break;
  // }

  // DISPLAY TIME
  // cout << "\nTotal Time:" << totalTime;
  // clockStart();
  // sleep(10);
  // clockStop();
  // cout << fixed << "\nTotal Time:" << (float)totalTime / CLOCKS_PER_SEC << " sec" << scientific;

  return 0;
}

// Parse .DAT file
vector<uint32_t> readFile()
{
  vector<uint32_t> result;
  // OPEN file
  string line, input = "input.dat";
  ifstream file(input);
  if (!file)
    cout << "ERROR. Cannot open file";
  else
  {
    cout << "RISCV SIMULATOR\n"
         << "\n~~ Loading instructions ~~\n";
    // READ file
    string str = "";
    string binary;
    while (file >> binary)
    {
      str = binary + str;
      if (str.length() == 32)
      {
        uint32_t val = bitset<32>(str).to_ulong(); // Convert string to binary
        result.push_back(val);                     // save instruction
        cout << str << endl;
        str.clear();
      }
    }
    file.close();
    cout << "~~ Loading complete ~~\n";
  }
  return result;
}

// Write to .ASM file for Debugging
void writeFile()
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
