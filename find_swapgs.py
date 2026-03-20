#!/usr/bin/env python3
#coded for linux kernel 6.17.0-5 
import struct, subprocess, sys

with open(sys.argv[1], 'rb') as f:
    data = f.read()

KERNEL_BASE = 0xffffffff80e00000
swapgs_op   = bytes.fromhex('0f01f8')
iretq_op    = bytes.fromhex('48cf')

pos = 0
candidates = []
while True:
    pos = data.find(swapgs_op, pos)
    if pos == -1:
        break
    va = KERNEL_BASE + pos
    if 0xffffffff81000000 <= va <= 0xffffffff82637970:
        candidates.append(va)
    pos += 1

print(f'[*] Total swapgs candidates at .text: {len(candidates)}')

valid = []
for va in candidates:
    file_off = va - KERNEL_BASE
    chunk = data[file_off : file_off + 0x100]
    if iretq_op in chunk:
        valid.append(va)

print(f'[*] Best Candidates: {len(valid)}')
for va in valid:
    print(f'    {hex(va)}')
print()

if valid:
    best = min(valid)
    print(f'[+] SWAPGS_RESTORE : {hex(best)}')
