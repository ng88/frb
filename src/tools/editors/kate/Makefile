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


TRAD_PATH=../../../trad/


all:  highlighter

.PHONY: .highlighter

highlighter: $(TRAD_PATH)/frbkeywords.key
	@echo Creating kate syntax highlighter...
	@awk -f create_kate_syntax_highlighter.awk  -- $(TRAD_PATH)frbkeywords.key > frb.xml

