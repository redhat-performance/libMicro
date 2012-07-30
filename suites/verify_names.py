#!/usr/bin/env python

#
# Copyright 2012 Red Hat, Inc.
#
# The contents of this file are subject to the terms
# of the Common Development and Distribution License
# (the "License").  You may not use this file except
# in compliance with the License.
#
# See the License for the specific language governing
# permissions and limitations under the License.
#

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
