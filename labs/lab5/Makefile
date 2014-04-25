all: pass_init pass_mp pass_tbb pass_cilk

default: pass_init pass_mp pass_tbb pass_cilk

pass_mp: pass_mp.c
	icc -std=c99 -openmp -o pass_mp pass_mp.c -lcrypto

pass_tbb: pass_tbb.cpp
	icpc -ltbb -o pass_tbb pass_tbb.cpp -lcrypto

pass_cilk: pass_cilk.c
	icc -std=c99 -o pass_cilk pass_cilk.c -lcilkrts -lcrypto

pass_init: pass_init.c
	icc -std=c99 -o pass_init pass_init.c -lcrypto

clean:
	rm *.o pass_init pass_mp pass_tbb pass_cilk
