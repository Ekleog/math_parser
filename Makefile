CXXFLAGS=-std=c++0x -Wall -Wextra -pedantic -O3 -flto

OBJ=obj/main.o obj/Parse.o obj/Expression.o obj/Split.o obj/Context.o

HEADERS=Expression.hpp Op0.hpp Op1.hpp Op2.hpp Parse.hpp

parser: $(OBJ)
	g++ $(CXXFLAGS) $(OBJ) -o parser

obj/%.o: %.cpp $(HEADERS)
	g++ $(CXXFLAGS) -c $< -o $@

.PHONY: clear
clear:
	rm -f $(OBJ)

mrproper: clear
	rm -f parser

