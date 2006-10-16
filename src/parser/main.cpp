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
#include "frbparser.h"

using namespace std;

inline void addClass(FrBParser::Tree * tree, FrBClass * c) { (*tree)[c->name()] = c; }


int main(int argc, char ** argv)
{
    /******** Anaylse des paramètres ********/

    enum { SHOW_TREE = 0, SHOW_MEM, SHOW_STACK, EXEC, SWITCH_COUNT };
    bool args_switch[SWITCH_COUNT];
    
    args_switch[SHOW_TREE] = true;
    args_switch[SHOW_MEM] = true;
    args_switch[SHOW_STACK] = true;
    args_switch[EXEC] = true;
    
    char * arg_main_class = "Main";
    char * arg_main_function = "main";
    int    arg_int_param = -1;
    
    for(int i = 0; i < argc; ++i)
    {
        if(!strcmp(argv[i], "--no-tree"))
            args_switch[SHOW_TREE] = false;
        else if(!strcmp(argv[i], "--no-mem"))
            args_switch[SHOW_MEM] = false;
        else if(!strcmp(argv[i], "--no-stack"))
            args_switch[SHOW_STACK] = false;
        else if(!strcmp(argv[i], "--no-exec"))
            args_switch[EXEC] = false;
        else if(!strcmp(argv[i], "--main-class") && i + 1 < argc)
            arg_main_class = argv[++i];
        else if(!strcmp(argv[i], "--main-function") && i + 1 < argc)
            arg_main_function = argv[++i];
        else if(!strcmp(argv[i], "--int-arg") && i + 1 < argc)
            arg_int_param = atois(argv[++i]); 
    }
    
    /******** Parsage ********/

    FrBParser parser;
    FrBParser::Tree * tree = parser.classList();
    
    
    try
    {
        addClass(tree, FrBInt::initClass());
        addClass(tree, FrBString::initClass());
        addClass(tree, FrBClassWrapper::initClass());
        addClass(tree, FrBFunctionWrapper::initClass());
        addClass(tree, FrBDebug::initClass());
        
        
        if(!parser.parse())
            return 1;

        if(args_switch[SHOW_TREE])
            parser.printTree();
            
        /******** Amélioration de l'arbre ********/

        parser.resolveAndCheckTree();
        
        /******** Interprétation/exécution ********/
            
        FrBMemory memory(100 * FrBMemory::B, 10 * FrBMemory::B, 2, 0);
        FrBExecutionEnvironment env(&memory);

        
        FrBClass * main = FrBClass::getClassFromString(arg_main_class);
        
        if(args_switch[EXEC])
        {
    
            cout << "Call to " << arg_main_class << "::" << arg_main_function;
            
            if(arg_int_param > -1) 
                cout << "(FrBInt = " << arg_int_param << ")...\n";
            else
                cout << "()...\n";

            FrBBaseObjectList args;
            
            FrBInt param0(arg_int_param);
            
            if(arg_int_param > -1)
                args.push_back(&param0);
            
            main->executeFunction(env, arg_main_function, 0, args);

            if(args_switch[SHOW_MEM])
            {
                cout << "Mem after call:\n";
                memory.print(1);
            }
            
            if(args_switch[SHOW_STACK])
                env.stack().print();
        }
        
    }
    catch(const FrBExecutionException &e)
    {
        cerr << '`' << typeid(e).name() <<  "' throwed\n";
        cerr << '\t' << e << '\n';
    }
    
      
    return 0;

}
