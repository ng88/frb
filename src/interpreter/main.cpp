/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006 by GUILLAUME Nicolas                               *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://frb.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   See the COPYING file.                                                 *
 ***************************************************************************/




#include <iostream>
#include "../parser/frbparser.h"
#include "../parser/frbmemory.h"
#include "frbbuiltinclasses.h"

using namespace std;

int main(int argc, char ** argv)
{

    FrBParser parser;
    FrBParser::Tree * tree;
    
    
    
    if(!parser.parse())
        return 1;
        
  
    tree = parser.classList();
    
    FrBMemory::initMemory(tree);
    
    
    (*tree)["Int"] = FrBInt::initClass();
     
    parser.printTree();
    
    FrBClass * main = (*tree)["Main"];
    frb_assert2(main != 0, "no Main class found");
    
    cout << "Mem before Main::main() call:\n";
    FrBMemory::getMemory()->print();
    
    main->callFunction("main", 0);

    cout << "Mem after Main::main() call:\n";
    FrBMemory::getMemory()->print(1);
           
    return 0;

}
