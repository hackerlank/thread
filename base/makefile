cc = g++

CFLAGS = -g

.PHONY:all
all: base

test: error.cpp error_test.cpp
	cc $(CFLAGS) -o test.app error.cpp error_test.cpp

base: error.cpp
	cc $(CFLAGS) -c error.cpp

.PHONY:clean
clean:
	-rm -f *.app
	-rm -f *.o
	-rm -rf *.dSYM
