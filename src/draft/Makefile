
CXX=g++
CXXFLAGS=-Wall

SRC= $(wildcard *.cpp)
OBJS= $(SRC:.cpp=.o)

EXE=frb

all: $(EXE)

$(EXE): $(OBJS)
	@echo Linking $@...
	@$(CXX) $^ -o $@ $(CXXFLAGS)
	@echo Done.


%.o: %.cpp
	@echo Compiling $<...
	@$(CXX) -c $< $(CXXFLAGS)


frbmem.o: frbmem.h frbmem.cpp frbobjectfactory.o
frbobjectfactory.o: frbobjectfactory.cpp frbobjectfactory.h string.o
string.o: string.cpp string.h
frbtypes.o: frbobjectfactory.o frbtypes.cpp frbtypes.h frbobjectfactory.o
frbinterpreter.o: frbinterpreter.h frbinterpreter.cpp events.h frbmisc.h frbparser.o
main.o: main.cpp frbinterpreter.o
frbparser.o: frbparser.h frbparser.cpp parser
frbkeywords.o: frbkeywords.h frbkeywords.cpp
parser: syntaxer.y lexer.y frbparser.h frbkeywords.o

touchall:
	@touch $(SRC) *.y

force: touchall all

clean:
	@rm -f *.o *~ core *.core core.*


parser:
	@echo Creating syntaxical parser...
	@rm -f syntaxer.cpp syntaxer.h syntaxer.o
	@bison++ -dv syntaxer.y && mv -f syntaxer.tab.c syntaxer.cpp && mv -f syntaxer.tab.h syntaxer.h
	@echo Creating lexical parser...
	@rm -f lexer.cpp
	@flex++ -PFrB lexer.y && mv -f lex.FrB.cc lexer.cpp
        

install: all
	@echo "Nothing to do"
