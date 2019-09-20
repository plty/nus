#!/usr/bin/env python3
from string import ascii_lowercase as asl
from itertools import product

for s in product(asl, repeat=5):
    print(''.join(s))
