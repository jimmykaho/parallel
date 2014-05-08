default: all

all: badIndex tbbNotQuiteRight

badIndex: badIndex.c
	icc -g -o badIndex badIndex.c -fopenmp

tbbNotQuiteRight: tbbNotQuiteRight.cpp
	icc -g -o tbbNotQuiteRight tbbNotQuiteRight.cpp -ltbb

clean:
	rm tbb badIndex tbbNotQuiteRight *.o 2>/dev/null
