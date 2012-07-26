SUITE_NAMES =	extended \
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
	./check

export LC_ALL=C
%.txt:	%.cpp
	$(CPP) -P $< | sort -k 2 | uniq > $@

full.txt: hardware.cpp syscalls.cpp libc.cpp memcpy.cpp librt.cpp libm.cpp filesystem.cpp readwrite.cpp network.cpp forkexec.cpp memory.cpp pipe.cpp localtcp.cpp

extended.txt: full.cpp hardware.cpp syscalls.cpp libc.cpp memcpy.cpp librt.cpp libm.cpp filesystem.cpp readwrite.cpp network.cpp forkexec.cpp memory.cpp pipe.cpp localtcp.cpp

filesystem.txt: readwrite.cpp

libc.txt: memcpy.cpp

network.txt: localtcp.cpp

pipe.txt: localtcp.cpp

clean:
	rm -f *~ *.txt