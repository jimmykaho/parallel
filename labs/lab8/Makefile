all: stencil_serial stencil_openmp stencil_cilk

stencil_serial: stencil_serial.cpp
	icpc -std=c++11 -o stencil_serial stencil_serial.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm

stencil_openmp: stencil_openmp.cpp
	icpc -std=c++11 -o stencil_openmp stencil_openmp.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm  -fopenmp

stencil_cilk: stencil_cilk.cpp
	icpc -std=c++11 -o stencil_cilk stencil_cilk.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm  -lcilkrts

clean:
	rm -f *.o stencil_serial stencil_openmp stencil_cilk
	
.PHONY: clean
