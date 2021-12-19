CPP=g++
CPPFLAGS=-std=c++17 -Wall -Wextra -Wpedantic -Weffc++ # -fmax-errors=1

all: Calculator.cc Parser.o Expression.o Node.o
	$(CPP) $(CPPFLAGS) Calculator.cc Parser.o Expression.o Node.o -o calc

Parser.o: Parser.h Parser.cc
	$(CPP) $(CPPFLAGS) -c Parser.cc -o Parser.o

Expression.o: Expression.h Expression.cc
	$(CPP) $(CPPFLAGS) -c Expression.cc -o Expression.o

Node.o: Node.h Node.cc
	$(CPP) $(CPPFLAGS) -c Node.cc -o Node.o

.PHONY: clean
clean:
	rm -f *.o
