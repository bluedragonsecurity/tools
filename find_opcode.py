#!/usr/bin/env python3
#coded for linux kernel 6.17.0-5 
import sys

with open(sys.argv[1], 'rb') as f:
    data = f.read()

KERNEL_BASE_VA  = 0xffffffff80e00000
needle = b'\x5f\xc3'  # opcode finterprint for : pop rdi ; ret 
results = []
pos = 0
while True:
    pos = data.find(needle, pos)
    if pos == -1:
        break
    va = KERNEL_BASE_VA + pos
    if 0xffffffff81000000 <= va <= 0xffffffff82637970:
        results.append(hex(va))
    pos += 1

print(f'Found {len(results)} gadgets')
for r in results[:10]:
    print(r)
