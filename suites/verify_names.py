#!/usr/bin/env python

import sys
from collections import defaultdict

lines = open(sys.argv[1], "r").readlines()
names = defaultdict(int)
for l in lines:
    vals = l.split(' ')
    if len(vals) < 2:
        continue
    name = vals[1]
    names[name] += 1

conflict = 0
for k,v in names.items():
    if v > 1:
        conflict = 1
        print k, v

sys.exit(conflict)
