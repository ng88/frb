###########################################################################
#   This file is part of the FrB project                                  #
#                                                                         #
#   FrB project                                                           #
#                                                                         #
#   Copyright (C) 2006 - 2010 by GUILLAUME Nicolas                        #
#   ng@ngsoft-fr.com                                                      #
#                                                                         #
#   http://frb.ngsoft-fr.com/                                             #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   See the COPYING file.                                                 #
###########################################################################


SRC= $(wildcard *.cpp)
OBJS= $(SRC:.cpp=.o)

EXE=frbc
TRAD_PATH=../trad/
GET_DEP=../get_dependencies.sh


all: $(EXE)

start:
	@echo Parser compilation started, language: $(TRAD_LANG)

$(EXE): start $(OBJS)
	@echo Linking $@...
	@$(CXX) *.o ../common/*.o -o $@ $(CXXFLAGS)
	@echo Striping $@...
	@$(STRIP) $@
	@echo Done.

# generated by 'make mydep'
##################################################
frbbaseobject.o: ../common/string.h ../common/assert.h frbexceptions.h frbfield.h frbbaseobject.h frbfield.h
frbbuiltinclasses.o: frbcppbinding.h frbmemory.h builtin/object.dec builtin/primitive.dec builtin/boolean.dec builtin/debug.dec builtin/null.dec builtin/void.dec frbbuiltinclasses.h ../common/assert.h frbkeywords.h frbclass.h builtin/object.def builtin/primitive.def builtin/boolean.def builtin/int.def builtin/string.def builtin/class.def builtin/function.def builtin/debug.def builtin/null.def builtin/void.def
frbclass.o: ../common/assert.h ../common/string.h ../common/iterators.h frbbaseobject.h frbevent.h frbfunction.h frbfield.h frbkeywords.h frbclass.h frbmemory.h frbkeywords.h ../common/assert.h ../common/misc.h frbbuiltinclasses.h frbexpr.h frbtemplatespecializationenvironment.h
frbcppbinding.o: frbclass.h frbbaseobject.h ../common/assert.h frbcppbinding.h frbbuiltinclasses.h ../common/assert.h ../common/misc.h
frberrordesc.o: ../common/string.h frberrors.h frberrordesc.h frbkeywords.h error_msg keywords_db
frbevent.o: frbevent.h
frbexceptions.o: ../common/string.h frbexprlist.h frbexceptions.h frbkeywords.h frberrors.h frbbaseobject.h frbclass.h frbmember.h frbexpr.h ../common/assert.h
frbexecutionenvironment.o: ../common/assert.h frbbaseobject.h frbmemory.h frbexecutionenvironment.h frbresolveenvironment.h frbbuiltinclasses.h frbfunction.h
frbexpr.o: frbbuiltinclasses.h frbexceptions.h frbexecutionenvironment.h frbexprlist.h frbtemplatepool.h frbexpr.h ../common/assert.h ../common/misc.h frbkeywords.h frbfunction.h frbresolveenvironment.h frbbuiltinclasses.h frbtemplatespecializationenvironment.h frbtemplatepool.h
frbfield.o: frbexceptions.h ../common/assert.h frbmember.h frbexprlist.h frbfield.h frbexpr.h frbclass.h frbkeywords.h frbexecutionenvironment.h ../common/misc.h frbtemplatespecializationenvironment.h
frbfunction.o: frbstat.h frbexceptions.h ../common/assert.h frbexprlist.h frbmember.h frbexpr.h frbfunction.h frbclass.h ../common/assert.h ../common/misc.h frbexecutionenvironment.h frbkeywords.h frbbuiltinclasses.h frbtemplatespecializationenvironment.h
frbmember.o: ../common/assert.h ../common/string.h frbconstants.h frbresolveenvironment.h frbmember.h frbkeywords.h frbclass.h frbtemplatespecializationenvironment.h frbstat.h
frbmemory.o: frbbaseobject.h ../common/assert.h frbmemory.h frbclass.h
frbparser.o: ../common/string.h frblexer.h frbsyntaxer.h syntaxer.h frbtemplatepool.h frbparser.h ../common/assert.h parser
frbresolveenvironment.o: frbfunction.h frbresolveenvironment.h frbclass.h ../common/assert.h ../common/string.h frbkeywords.h
frbstat.o: ../common/string.h frbbaseobject.h frbexecutionenvironment.h frbexceptions.h frbstat.h frbexpr.h frbclass.h ../common/assert.h ../common/misc.h frbmemory.h frbbuiltinclasses.h frbkeywords.h frbtemplatespecializationenvironment.h
frbtemplatepool.o: ../common/assert.h frbexceptions.h frbtemplatepool.h frbclass.h frbexpr.h frbresolveenvironment.h
frbtemplatespecializationenvironment.o: frbfunction.h frbtemplatespecializationenvironment.h frbclass.h ../common/assert.h
main.o: frbparser.h frbresolveenvironment.h frbkeywords.h frbmemory.h frbtemplatespecializationenvironment.h
##################################################

.PHONY: exe clean install touchall force doc cleandoc start mydep

# list the dependencies
mydep:
	@$(MAKE) -C .. clean
	@echo '################### DEPENDENCIES ###################'
	@$(GET_DEP) $$(ls *.cpp | grep -v frbkeywords.cpp | grep -v frberrors.cpp | grep -v lexer | grep -v syntaxer)
	@echo '####################################################'
doc:
	@echo Creating documentation...
	@doc++ -AHabj -d ../../docs/code-doc *.h *.cpp builtin/*.def builtin/*.dec ../common/*.h ../common/*.cpp

cleandoc:
	@echo Cleaning documentation...
	@rm -fR ../../docs/code-doc

exe:
	@./$(EXE)

touchall:
	@touch $(SRC) $(OBJS) *.y *.l ../trad/*.key error_msg parser keywords_db

force: touchall all

clean:
	@echo Cleaning parser...
	@rm -f *.o *~ core *.core core.* *.tmp lexer.cpp *.output error_msg parser keywords_db
	@rm -f syntaxer.cpp syntaxer.h frbkeywords.h frbkeywords.cpp
	@rm -f frberrors.h frberrors.cpp
	@rm -f syntaxer_kw.y lexer_kw.l


syntaxer.h: parser
parser: frberrordesc.h syntaxer.y lexer.l frbexpr.h frbstat.h frbfunction.h frbclass.h frbsyntaxer.h
	@echo Defining syntaxical parser keyword token...
	@awk -f create_keywords_db_syntaxer.awk  -- $(TRAD_PATH)frbkeywords.key > token_list.tmp
	@./insert.sh '%DECLARE_KEYWORDS_TOKEN' syntaxer_kw.y syntaxer.y token_list.tmp
	@echo Creating syntaxical parser...
	@rm -f syntaxer.cpp syntaxer.h syntaxer.o
	@bison++ -dv syntaxer_kw.y && mv -f syntaxer_kw.tab.c syntaxer.cpp && mv -f syntaxer_kw.tab.h syntaxer.h
	@sed -i 's/ERROR(char \*msg)/ERROR(const char \*msg)/g' syntaxer.cpp     # PATCH for GCC 4.2.3	
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
	@touch parser

frbkeywords.h: keywords_db
keywords_db: $(TRAD_PATH)/frbkeywords.key
	@echo Creating keywords tables...
	@awk -f create_keywords_db_header.awk  -- $(TRAD_PATH)frbkeywords.key > frbkeywords.h
	@awk -f create_keywords_db_source.awk  -- $(TRAD_PATH)frbkeywords.key > frbkeywords.cpp
	@echo Compiling keywords tables...
	@$(CXX) -c frbkeywords.cpp $(CXXFLAGS)
	@touch keywords_db

frberrors.h: error_msg
error_msg: $(TRAD_PATH)/frberrors.$(TRAD_LANG).key
	@echo "Creating error messages table ($(TRAD_LANG))..."
	@awk -f create_errorsmsg_header.awk  -- $(TRAD_PATH)frberrors.$(TRAD_LANG).key > frberrors.h
	@awk -f create_errorsmsg_source.awk  -- $(TRAD_PATH)frberrors.$(TRAD_LANG).key > frberrors.cpp
	@echo Compiling error messages table...
	@$(CXX) -c frberrors.cpp $(CXXFLAGS)
	@touch error_msg


install: all
	@echo "Nothing to do"

%.o: %.cpp
	@echo Compiling $<...
	@$(CXX) -c $< $(CXXFLAGS)

