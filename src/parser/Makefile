###########################################################################
#   This file is part of the FrB project                                  #
#                                                                         #
#   FrB project                                                           #
#                                                                         #
#   Copyright (C) 2006 by GUILLAUME Nicolas                               #
#   ng@ngsoft-fr.com                                                      #
#                                                                         #
#   http://frb.ngsoft-fr.com/                                             #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#   See the COPYING file.                                                 #
###########################################################################


SRC= $(wildcard *.cpp)
OBJS= $(SRC:.cpp=.o)
BUILTIN_DEF= $(wildcard builtin/*.def)
BUILTIN_DEC= $(wildcard builtin/*.dec)

EXE=parser_test
TRAD_PATH=../trad/



all: $(EXE)

start:
	@echo Parser compilation started, language: $(TRAD_LANG)

$(EXE): start main.o
	@echo Linking $@...
	@$(CXX) *.o ../common/*.o -o $@ $(CXXFLAGS)
	@echo Striping $@...
	@$(STRIP) $@
	@echo Done.



main.o: frbparser.o
frbparser.o: frbparser.h frbparser.cpp parser
frberrordesc.o: error_msg keywords_db
frbclass.o: frbclass.h frbclass.cpp frbbaseobject.o frbfunction.o frbfield.o
frbexpr.o: frbexpr.h frbexpr.cpp frbbaseobject.o ../common/assert.h frbbuiltinclasses.o frbexecutionenvironment.o
frbstat.o: frbstat.h frbstat.cpp frbbaseobject.o frbmemory.o frbclass.h frbclass.cpp ../common/assert.h frbexceptions.o
frbfunction.o: frbfunction.h frbfunction.cpp frbstat.o frbmember.o
frbbaseobject.o: frbbaseobject.h frbbaseobject.cpp
frbmemory.o: frbmemory.h frbmemory.cpp
frbbuiltinclasses.o: frbbuiltinclasses.h frbbuiltinclasses.cpp frbmemory.o frbcppbinding.o
frbcppbinding.o: frbcppbinding.h frbcppbinding.cpp frbclass.o ../common/assert.h frbbaseobject.o
frbexecutionenvironment.o: frbexecutionenvironment.h frbexecutionenvironment.cpp frbmemory.o frbresolveenvironment.o
frbresolveenvironment.o: frbresolveenvironment.h frbresolveenvironment.cpp
frbexceptions.o: frbexceptions.h frbexceptions.cpp
frbbuiltinclasses.h: $(BUILTIN_DEC)
frbbuiltinclasses.cpp: $(BUILTIN_DEF)
frbfield.o: frbfield.h frbfield.cpp frbmember.o
frbmember.o: frbmember.h

.PHONY: exe clean install touchall force doc cleandoc

doc:
	@echo Creating documentation...
	@doc++ -AHabj -d ../../docs/code-doc *.h *.cpp builtin/*.def builtin/*.dec ../common/*.h ../common/*.cpp

cleandoc:
	@echo Cleaning documentation...
	@rm -fR ../../docs/code-doc

exe:
	@./$(EXE)

touchall:
	@touch $(SRC) $(OBJS) *.y *.l ../trad/*.key

force: touchall all

clean:
	@echo Cleaning parser...
	@rm -f *.o *~ core *.core core.* *.tmp lexer.cpp *.output
	@rm -f syntaxer.cpp syntaxer.h frbkeywords.h frbkeywords.cpp
	@rm -f frberrors.h frberrors.cpp
	@rm -f syntaxer_kw.y lexer_kw.l



parser: frberrordesc.o syntaxer.y lexer.l frbexpr.o frbstat.o frbfunction.o frbclass.o
	@echo Defining syntaxical parser keyword token...
	@awk -f create_keywords_db_syntaxer.awk  -- $(TRAD_PATH)frbkeywords.key > token_list.tmp
	@./insert.sh '%DECLARE_KEYWORDS_TOKEN' syntaxer_kw.y syntaxer.y token_list.tmp
	@echo Creating syntaxical parser...
	@rm -f syntaxer.cpp syntaxer.h syntaxer.o
	@bison++ -dv syntaxer_kw.y && mv -f syntaxer_kw.tab.c syntaxer.cpp && mv -f syntaxer_kw.tab.h syntaxer.h
	@echo Creating lexical parser symbol rules...
	@awk -f create_keywords_db_lexer.awk  -- $(TRAD_PATH)frbkeywords.key > symbols_rules.tmp
	@./insert.sh '%DECLARE_SYMBOL_RULES' lexer_kw.l lexer.l symbols_rules.tmp
	@echo Creating lexical parser...
	@rm -f lexer.cpp
	@flex++ -PFrB lexer_kw.l && mv -f lex.FrB.cc lexer.cpp
	@echo Compiling lexer...
	@$(CXX) -c lexer.cpp $(CXXFLAGS) -Wno-unused
	@echo Compiling syntaxer...
	@$(CXX) -c syntaxer.cpp $(CXXFLAGS)

keywords_db: $(TRAD_PATH)/frbkeywords.key
	@echo Creating keywords tables...
	@awk -f create_keywords_db_header.awk  -- $(TRAD_PATH)frbkeywords.key > frbkeywords.h
	@awk -f create_keywords_db_source.awk  -- $(TRAD_PATH)frbkeywords.key > frbkeywords.cpp
	@echo Compiling keywords tables...
	@$(CXX) -c frbkeywords.cpp $(CXXFLAGS)

error_msg: $(TRAD_PATH)/frberrors.$(TRAD_LANG).key
	@echo "Creating error messages table ($(TRAD_LANG))..."
	@awk -f create_errorsmsg_header.awk  -- $(TRAD_PATH)frberrors.$(TRAD_LANG).key > frberrors.h
	@awk -f create_errorsmsg_source.awk  -- $(TRAD_PATH)frberrors.$(TRAD_LANG).key > frberrors.cpp
	@echo Compiling error messages table...
	@$(CXX) -c frberrors.cpp $(CXXFLAGS)


install: all
	@echo "Nothing to do"

highlighter: $(TRAD_PATH)/frbkeywords.key
	@echo Creating kate syntax highlighter...
	@awk -f create_kate_syntax_highlighter.awk  -- $(TRAD_PATH)frbkeywords.key > frb.xml

%.o: %.cpp
	@echo Compiling $<...
	@$(CXX) -c $< $(CXXFLAGS)

