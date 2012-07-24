SUITE_NAMES =	filesystem \
				forkexec \
				full \
				hardware \
				libc \
				libm \
				librt \
				memcpy \
				memory \
				network \
				pipe \
				readwrite \
				threading

default: $(SUITE_NAMES:%=%.txt)

%.txt:	%.cpp
	$(CPP) -P $< -o $@

clean:
	rm -f *~ *.txt