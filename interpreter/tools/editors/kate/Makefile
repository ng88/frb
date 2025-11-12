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

FNAME=frb.xml

all:  highlighter

.PHONY: .highlighter

highlighter: $(TRAD_PATH)/frbkeywords.key
	@echo Creating kate syntax highlighter $(FNAME)...
	@awk -f create_kate_syntax_highlighter.awk  -- $(TRAD_PATH)frbkeywords.key > $(FNAME)
	@echo Done. $(FNAME) must be moved to KDEDIR/share/apps/katepart/syntax/
	@echo '(usually /usr/share/apps/katepart/syntax/)'
