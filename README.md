# RISC-V-Simulator
## Progress

n/a = no tests available 

| Symbol  | Format                | Description                        | STATUS      |
| ------- | --------------------- | ---------------------------------- | ----------- |
| LUI     | `LUI rd,imm`          | Load Upper Immediate               |  |
| AUIPC   | `AUIPC rd,offset`     | Add Upper Immediate to PC          | n/a |
| JAL     | `JAL rd,offset`       | Jump and Link                      |  |
| JALR    | `JALR rd,rs1,offset`  | Jump and Link Register             | n/a |
| BEQ     | `BEQ rs1,rs2,offset`  | Branch Equal                       |  |
| BNE     | `BNE rs1,rs2,offset`  | Branch Not Equal                   | n/a |
| BLT     | `BLT rs1,rs2,offset`  | Branch Less Than                   | n/a |
| BGE     | `BGE rs1,rs2,offset`  | Branch Greater than Equal          |  |
| BLTU    | `BLTU rs1,rs2,offset` | Branch Less Than Unsigned          | n/a |
| BGEU    | `BGEU rs1,rs2,offset` | Branch Greater than Equal Unsigned | n/a |
| LB      | `LB rd,offset(rs1)`   | Load Byte                          |  |
| LH      | `LH rd,offset(rs1)`   | Load Half                          |  |
| LW      | `LW rd,offset(rs1)`   | Load Word                          |  |
| LBU     | `LBU rd,offset(rs1)`  | Load Byte Unsigned                 |  |
| LHU     | `LHU rd,offset(rs1)`  | Load Half Unsigned                 |  |
| SB      | `SB rs2,offset(rs1)`  | Store Byte                         |  |
| SH      | `SH rs2,offset(rs1)`  | Store Half                         |  |
| SW      | `SW rs2,offset(rs1)`  | Store Word                         |  |
| ADDI    | `ADDI rd,rs1,imm`     | Add Immediate                      | done |
| SLTI    | `SLTI rd,rs1,imm`     | Set Less Than Immediate            | done |
| SLTIU   | `SLTIU rd,rs1,imm`    | Set Less Than Immediate Unsigned   |  |
| XORI    | `XORI rd,rs1,imm`     | Xor Immediate                      | done |
| ORI     | `ORI rd,rs1,imm`      | Or Immediate                       | done |
| ANDI    | `ANDI rd,rs1,imm`     | And Immediate                      | done |
| SLLI    | `SLLI rd,rs1,imm`     | Shift Left Logical Immediate       | done |
| SRLI    | `SRLI rd,rs1,imm`     | Shift Right Logical Immediate      | done |
| SRAI    | `SRAI rd,rs1,imm`     | Shift Right Arithmetic Immediate   | done |
| ADD     | `ADD rd,rs1,rs2`      | Add                                | done |
| SUB     | `SUB rd,rs1,rs2`      | Subtract                           | done |
| SLL     | `SLL rd,rs1,rs2`      | Shift Left Logical                 | done |
| SLT     | `SLT rd,rs1,rs2`      | Set Less Than                      | done |
| SLTU    | `SLTU rd,rs1,rs2`     | Set Less Than Unsigned             | done |
| XOR     | `XOR rd,rs1,rs2`      | Xor                                | done |
| SRL     | `SRL rd,rs1,rs2`      | Shift Right Logical                | done |
| SRA     | `SRA rd,rs1,rs2`      | Shift Right Arithmetic             | done |
| OR      | `OR rd,rs1,rs2`       | Or                                 | done |
| AND     | `AND rd,rs1,rs2`      | And                                | done |
