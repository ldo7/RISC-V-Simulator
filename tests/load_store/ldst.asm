addi t4, x0, 0x100          # Load address 0x80000000 in t4
slli t4, t4, 20
addi t3, x0, 0x100
slli t3, t3, 8
add t4, t3, t4
addi t0, x0, -1             # Store m = -1 at 0x80000000
sw t0, 0(t4)
addi t0, t0, 8              # Store x = 7 at 0x80000004
sw t0, 4(t4) 
addi t0, x0, -5             # Store c = -5 at 0x80000008
sw t0, 8(t4) 
lw t0, 0(t4)                # Load m from 0x80000000
lw t1, 4(t4)                # Load x from 0x80000004
lw t2, 8(t4)                # Load c from 0x80000008
and t3, t0, t1              # y = (m and x) or c
or t3, t3, t2 
addi t5, x0, 0x5a
slli t5, t5, 8
addi t5, t5, 0x0f
slli t5, t5, 8
addi t5, t5, 0xa5
slli t5, t5, 8
addi t5, t5, 0xf0
xor t3, t3, t5
sw t3, 12(t4)               #store it in y
addi t4, t4, 12
lb t0, 3(t4)                # Load signed and unsigned bytes
sb t0, 1(t4)
lbu t0, 0(t4)
sb t0, 2(t4)
lh t0, 2(t4)                # Load signed and unsigned halfwords 
sh t0, 4(t4)
lhu t0, 0(t4)
sh t0, 6(t4)

nop
