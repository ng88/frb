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



ifndef TRAD_LANG
    TRAD_LANG=EN
endif

MAKE_OPT=TRAD_LANG=$(TRAD_LANG)


EXE=interpreter_test



all: $(EXE)

$(EXE): interpreter
	@ln -s interpreter/$(EXE)




.PHONY: exe clean force clean_parser clean_interpreter force_parser force_interpreter parser

exe:
	@./$(EXE)

clean_parser:
	@$(MAKE) -C parser $(MAKE_OPT) clean


clean_interpreter: force_parser
	@$(MAKE) -C interpreter $(MAKE_OPT) clean

force_parser:
	@$(MAKE) -C parser $(MAKE_OPT) force


force_interpreter:
	@$(MAKE) -C interpreter $(MAKE_OPT) force

force: force_interpreter

clean: clean_parser clean_interpreter



parser:
	@$(MAKE) -C parser $(MAKE_OPT)
        
interpreter: parser
	@$(MAKE) -C interpreter $(MAKE_OPT)


install: all
	@echo "Nothing to do"


    
