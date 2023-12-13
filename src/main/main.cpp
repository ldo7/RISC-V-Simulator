#include <iostream>
#include <fstream>
#include <bitset>
#include <ctime>
#include <vector>
#include <unistd.h>
#include <string.h>
#include "../cpu/cpu.cpp"
using namespace std;

vector<uint32_t> readFile(string filename);
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
  
  userInput();  // REQUEST USER INPUT
  string filename = "input.dat";
  vector<uint32_t> instructions = readFile(filename); // LOAD INSTRUCTIONS 
  cpu cpu(instructions);                      // INTANTIATE & RUN CPU
  cpu.run();
  // bool cpu_is_running = true;
  // while (true)
  // {
 
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
vector<uint32_t> readFile(string filename)
{
  vector<uint32_t> result;
  // OPEN file
  string line, input = filename;;
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
        // cout << str << endl;
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
  string command = input.substr(0, 1);
  int num;

  // VALIDATE INPUT
  //  run - execute all
  //  c - continue
  if (command == "r" || command == "c")
  {
    // clockStart();
  }
  // s - step through
  // else if (c == "s")
  // {
  // }

  // x0 - x31 - display the register
  else if (command == "x")
  {
    string regStr = input.substr(1);
      try
      {
        num = stoi(regStr);  // convert the input data type 
        if (num >= 0 && num <= 31)
          {
            cout << "Register x" << num << ": " << cpu.getReg(num) << endl;
          }
          else
            {
              cout << "Invalid register value\n";
            }
        }
        catch (invalid_argument const &e)
        {
            cout << "Invalid register value\n";
        }
  }
  // pc - return PC value
  
  else if (command == "pc")
    {
      cout << "PC = " << hex << cpu.getPC() << dec << endl;   // display the PC value
    }

  // 0x12345678 - return content at an address

  else if (command == "0")
    {
      string addrStr = input.substr(1);
      try
        {
        num = stoi(addrStr, nullptr, 16);
        cout << "Value at this address 0x" << hex << num << ": " << cpu.getMem(num) << dec << endl;
        }
      catch (invalid_argument const &e)
        {
          cout << "Invalid address\n";
        }
  }
// b[pc] - add breakpoint assume format -> b[0x12345678]

  else if (command == "b")
  {
      string addrStr = input.substr(2, 8);
      try
        {
        num = stoi(addrStr, nullptr, 16);
        // ASSUME SORTED IN-ORDER....
        for (int i = 0; i < 5; i++)
        {
          if (breakpoints[i] == 0xFFFFFFFF)  // check if breakpoints[] is full
          {
            breakpoints[i] = num;
            cout << "Breakpoint at address 0x" << hex << num << dec << endl;
            break;
          }
        }
      }
      catch (invalid_argument const &e)
      {
        cout << "Invaid address\n";
      }
    }
// insn - view next instruction
  else if (command == "insn")
    {
      cout << "Next instruction: " << cpu.getAsmInstruction() << endl; // use getAsmInstruction() return the next instruction
    }
    else {
      cout << "Invalid input\n";
    }
}
