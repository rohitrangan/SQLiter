# SQLiter - Makefile
# Author:- Rohit Rangan
#
# Makefile for the C++ code.

CXX=c++
CXXFLAGS= -Wall -Wextra
CXXSRCS=src/db.cpp src/fileio.cpp src/parser.tab.cpp# src/main.cpp
CXXOBJS=$(CXXSRCS:.cpp=.o) src/lex.yy.o
FLEXSRCS=src/lexer.l
FLEXOBJS=src/lex.yy.c
BISONSRCS=src/parser.ypp
BISONOBJS=src/parser.tab.cpp src/parser.tab.hpp
LIBS=
INCLUDES=-I./include
TARGET=sqliter
#DEBUGFLAGS=-g -ggdb -O0 -fno-inline

# Required for the target clean.
RM=rm -rf

CXXFLAGS+=$(DEBUGFLAGS) $(INCLUDES) $(LIBS)

all : $(BISONSRCS) $(BISONOBJS) $(FLEXSRCS) $(FLEXOBJS) $(CXXSRCS) $(CXXOBJS) $(TARGET)

$(BISONOBJS) : $(BISONSRCS)
	bison -d $< -o $@

$(FLEXOBJS) : $(FLEXSRCS)
	flex -o $@ $<

$(TARGET) : $(CXXOBJS)
	$(CXX) $(CXXOBJS) -o $(TARGET)

.c.o :
	$(CXX) $(CXXFLAGS) -c $< -o $@

.cpp.o :
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	$(RM) src/*.o src/*.dSYM $(TARGET) ./src/parser.tab.cpp ./src/parser.tab.hpp ./src/lex.yy.c

depend : .depend

.depend : $(CXXSRCS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^ -MF ./.depend

.PHONY : clean

include .depend
