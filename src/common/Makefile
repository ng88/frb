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


all: start $(OBJS)

start:
	@echo Compiling common stuff...




string.o: string.h string.cpp


.PHONY: clean

clean:
	@echo Cleaning...
	@rm -f *.o *~ core *.core core.* *.tmp lexer.cpp *.output

%.o: %.cpp
	@echo Compiling $<...
	@$(CXX) -c $< $(CXXFLAGS)

