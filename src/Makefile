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


EXE=frb



all: $(EXE)

$(EXE): common parser
	@ln -s parser/parser_test $(EXE)




.PHONY: exe clean clean_parser clean_common #clean_interpreter

exe:
	@./$(EXE)


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


    
