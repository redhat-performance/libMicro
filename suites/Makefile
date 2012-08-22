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

SUITE_NAMES =	extended \
				fifo \
				filesystem \
				forkexec \
				full \
				hardware \
				libc \
				libm \
				librt \
				localtcp \
				memcpy \
				memory \
				network \
				pipe \
				readwrite \
				syscalls \
				threading

default: $(SUITE_NAMES:%=%.txt)
	./verify_names.py extended.txt
	./check

export LC_ALL=C
%.txt:	%.cpp
	$(CPP) -x c -P $< | sort -k 2 | uniq > $@

full.txt: hardware.cpp syscalls.cpp libc.cpp memcpy.cpp librt.cpp libm.cpp filesystem.cpp readwrite.cpp network.cpp forkexec.cpp memory.cpp pipe.cpp localtcp.cpp fifo.cpp

extended.txt: full.cpp hardware.cpp syscalls.cpp libc.cpp memcpy.cpp librt.cpp libm.cpp filesystem.cpp readwrite.cpp network.cpp forkexec.cpp memory.cpp pipe.cpp localtcp.cpp fifo.cpp

filesystem.txt: readwrite.cpp fifo.cpp

libc.txt: memcpy.cpp

network.txt: localtcp.cpp

pipe.txt: localtcp.cpp fifo.cpp

clean:
	rm -f *~ *.txt