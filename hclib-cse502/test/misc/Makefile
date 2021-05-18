include $(HCLIB_ROOT)/include/hclib.mak
include $(HCLIB_ROOT)/include/likwid-gcc.mk

LIKWID_INCLUDE ?= -I/usr/local/include
LIKWID_LIBDIR ?= -L/usr/local/lib

TARGETS=fib nqueens fib-ddt Cilksort


all: clean $(TARGETS) clean-obj

%: %.cpp
	$(CXX) $(PROJECT_CXXFLAGS) $(PROJECT_LDFLAGS) $(LIKWID_INCLUDE) $(LIKWID_LIBDIR) -o $@ $^ $(PROJECT_LDLIBS)

clean-obj:
	rm -rf *.o

clean:
	rm -rf *.o $(TARGETS)
