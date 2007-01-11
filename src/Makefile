###########################################################################
#   This file is part of the FrB project                                  #
#                                                                         #
#   FrB project                                                           #
#                                                                         #
#   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         #
#   ng@ngsoft-fr.com                                                      #
#                                                                         #
#   http://frb.ngsoft-fr.com/                                             #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   See the COPYING file.                                                 #
###########################################################################



ifndef TRAD_LANG
    TRAD_LANG=EN
endif

ifdef NO_ASSERT
    DEBUGASSERT=
else
    DEBUGASSERT=-DDEBUG_ASSERT=1
endif

ifdef NO_CATCH
    DEBUGCATCH=-DDEBUG_NO_CATCH=1
else
    DEBUGCATCH=
endif


ifdef DEBUG
    DEBUGFLAGS=-g -ggdb -dH -D_FRB_DEBUG_=1 $(DEBUGASSERT) $(DEBUGCATCH)
    STRIP=echo '*** No strip in debug mode *** '
else
    DEBUGFLAGS=$(DEBUGASSERT)
    STRIP=strip
endif

CXX=g++
CXXFLAGS=-Wall -D_FRB_LANG_=\"$(TRAD_LANG)\" -D_FRB_LANG_IS_$(TRAD_LANG)_ $(DEBUGFLAGS)


MAKE_OPT=TRAD_LANG="$(TRAD_LANG)" CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" STRIP="$(STRIP)"

EXE=frb



all: $(EXE)

$(EXE): common parser
#	@cp -f parser/parser_test $(EXE)


.PHONY: exe clean clean_parser clean_common common parser #clean_interpreter

exe:
	@../bin/$(EXE)


####    common stuff      ####

clean_common:
	@$(MAKE) -C common $(MAKE_OPT) clean

common:
	@$(MAKE) -C common $(MAKE_OPT)

####     Parser   ####

clean_parser:
	@$(MAKE) -C parser $(MAKE_OPT) clean

parser:
	@$(MAKE) -C parser $(MAKE_OPT)

####     Interpreter    ####

# clean_interpreter: force_parser
# 	@$(MAKE) -C interpreter $(MAKE_OPT) clean
# 
# interpreter: parser
# 	@$(MAKE) -C interpreter $(MAKE_OPT)


clean: clean_parser clean_common #clean_interpreter





install: all
	@echo "Nothing to do"


    
