CXX = g++
CC = gcc
# This is the name of our compiler
BINARY = opc
LEX = flex
YACC = bison
DIFF = diff


# Flags
CFLAGS = -g -Wall -c 
CXXFLAGS = -g -Wall -D__STDC_LIMIT_MACROS
YACCFLAGS = -d -y -v 

CSOURCES = \
	symtab.c\
	usrdef.c \
	rulefuncs.c \
	error.c \
	shared.c \
	y.tab.c \
	lex.yy.c\

CXXSOURCES = \
	utils.cpp \
	code_gen.cpp \
	control_flow.cpp \
	main.cpp \




OBJECTS =   \
	symtab.o\
	usrdef.o \
	rulefuncs.o \
	error.o \
	shared.o \
	y.tab.o \
	lex.yy.o\

all: server


server: yacc lex
	$(CC) $(CSOURCES) $(CFLAGS)
	$(CXX) -std=c++0x $(OBJECTS) $(CXXSOURCES) -o server

# all:    yacc lex $(OBJECTS) tests_prepare
# 	g++ -o $(BINARY) $(CFLAGS) y.tab.c lex.yy.o main.o control_flow.o $(OBJECTS)  

# all: yacc lex $(OBJECTS) 
# 	g++ -o opc main.o control_flow.o -x c lex.yy.c y.tab.c $(OBJECTS)

lex:
	$(LEX) pascal.l

yacc: 
	$(YACC) $(YACCFLAGS) pascal.y
	@# Play tricks with yacc to prepend our shared definitions
	echo "#include \"shared.h\"" >> y.tab.h.tmp
	cat y.tab.h >> y.tab.h.tmp
	rm y.tab.h
	mv y.tab.h.tmp y.tab.h

# control_flow.o: control_flow.cpp
# 	g++ -c -std=c++0x control_flow.cpp

# main.o: main.cpp
# 	g++ -c main.cpp

# .c.o:
# 	$(CC) -c $(CFLAGS) $<

clean:
	-rm -f core $(BINARY) out.c *~
	-rm -f lex.yy.c y.tab.c y.tab.h y.tab.h.tmp y.output
	-rm -f $(OBJECTS)
	-rm -f main.o control_flow.o 
	-rm -f server
	-rm -f tests_lib_shared_auto.inc
	-rm -f tests_semantic/results/$(BINARY).{log,sum}
	-rm -f tests_semantic/*.output
	-rm -f tests_semantic/out.c
	-rm -f tests_semantic/a.out

tests: tests_semantic tests_opt tests_gen

tests_semantic: tests_prepare
	# "runtest" comes from DejaGNU
	mkdir -p tests_semantic/results
	(cd tests_semantic; ../dejagnu/runtest --all --output results --tool $(BINARY) tests_semantic.exp)

tests_prepare:
	@echo "BINARY=../$(BINARY)" > tests_lib_shared_auto.inc

.PHONY: all lex yacc clean tests
