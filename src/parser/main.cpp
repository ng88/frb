/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://frb.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/



#include <iostream>
#include <fstream>
#include "frbparser.h"
#include "frbresolveenvironment.h"
#include "frbkeywords.h"
#include "frbmemory.h"
#include "frbtemplatespecializationenvironment.h"

using namespace std;

inline void addClass(FrBParser::Tree * tree, FrBClass * c) { (*tree)[c->name()] = c; }
inline void addClassAlias(FrBParser::Tree * tree, FrBClass * c, String name) { (*tree)[name] = c; }

/** stupid shared initialization */
void sharedInit(const FrBClass::ClassContainer * classes, FrBExecutionEnvironment& e)
{
    for(FrBClass::ClassContainer::const_iterator it =  classes->begin(); it != classes->end(); ++it)
    {
	if(it->first == it->second->name())
	{
	    sharedInit(it->second->innerClassList(), e);
	  
	    if(it->second->sharedFieldCount() > 0)
		it->second->initSharedField(e);
	}
    }
}

void usage(char * exe)
{
    cerr << "usage: " << exe << " <parameter1> [... <parameterN>]" << endl
         << "   where parameter can be one of these:" << endl << endl
         << "     --parse-tree        print parse tree (not resolved neither optimized)" << endl
         << "     --improved-tree     print improved tree" << endl
         << "     --both-trees        print both parse and improved trees" << endl
         << "     --tree-set s        where s is the name of the keyword set to use (default: EN)" << endl
         << "     --all-classes       show all classes in tree (not only user defined class)" << endl
         << "     --mem               show heap memory after execution" << endl
         << "     --stack             show stack after execution" << endl
         << "     --no-exec           do not execute the parsed program" << endl
         << "     --main-class c      where c is the name of class (default: Main)" << endl
         << "                         containing the main function" << endl
         << "     --main-function f   where f is the name of main function (default: main)" << endl
         << "     --int-arg i         if defined, i will be passed as an Int parameter of " << endl
         << "                         the main function" << endl
         << "     --file f            where f is the path of the file to parse, if not" << endl
         << "                         defined, the standard input will be used" << endl
         << "     --                  explicit end of options" << endl;
}

int main(int argc, char ** argv)
{

#ifdef _FRB_DEBUG_
    char * debug_argv[] =
        {
            argv[0],
            "--file",
            "tests/with.frb",
        };
    if(argc == 1)
    {
        argv = debug_argv;
        argc = sizeof(debug_argv) / sizeof(debug_argv[0]);
    }
#endif

    /******** Analyse des paramètres ********/
    
    char * exename = strrchr(argv[0], '/');

    if(!exename) exename = argv[0]; else  exename++;

    enum { SHOW_PARSE_TREE = 0, SHOW_IMPROVED_TREE, SHOW_ALL_CLASSES,
            SHOW_MEM, SHOW_STACK, EXEC, SWITCH_COUNT };
            
    bool args_switch[SWITCH_COUNT];
    
    args_switch[SHOW_PARSE_TREE] = false;
    args_switch[SHOW_IMPROVED_TREE] = false;
    args_switch[SHOW_ALL_CLASSES] = false;
    args_switch[SHOW_MEM] = false;
    args_switch[SHOW_STACK] = false;
    args_switch[EXEC] = true;
    
    char *     arg_main_class = "Main";
    char *     arg_main_function = "main";
    int        arg_tree_set = FrBKeywords::FRB_SET_EN;
    istream *  arg_in = &cin;
    ifstream * arg_file = 0;
    int        arg_int_param = -1;
    
    
    
    for(int i = 1; i < argc; ++i)
    {
             if(!strcmp(argv[i], "--")) break; /* useless for the moment */
        else if(!strcmp(argv[i], "--parse-tree")) args_switch[SHOW_PARSE_TREE] = true;
        else if(!strcmp(argv[i], "--improved-tree")) args_switch[SHOW_IMPROVED_TREE] = true;
        else if(!strcmp(argv[i], "--both-trees")) args_switch[SHOW_PARSE_TREE] = args_switch[SHOW_IMPROVED_TREE] = true;
        else if(!strcmp(argv[i], "--all-classes")) args_switch[SHOW_ALL_CLASSES] = true;
        else if(!strcmp(argv[i], "--mem")) args_switch[SHOW_MEM] = true;
        else if(!strcmp(argv[i], "--stack")) args_switch[SHOW_STACK] = true;
        else if(!strcmp(argv[i], "--no-exec")) args_switch[EXEC] = false;
        else if(!strcmp(argv[i], "--main-class") && i + 1 < argc) arg_main_class = argv[++i];
        else if(!strcmp(argv[i], "--main-function") && i + 1 < argc) arg_main_function = argv[++i];
        else if(!strcmp(argv[i], "--int-arg") && i + 1 < argc)  arg_int_param = atoi(argv[++i]); 
        else if(!strcmp(argv[i], "--file") && i + 1 < argc) arg_file = new ifstream(argv[++i], ios::in);
        else if(!strcmp(argv[i], "--tree-set") && i + 1 < argc)
        {
	  int t = FrBKeywords::findSet(argv[++i]);
	  if(t > -1)
	    arg_tree_set = t;
	  else
	    cerr << '`' << argv[i] << "' is not a valid set name, ignored" << endl;
	}
	else
        {
            cerr << "invalid parameter `" << argv[i] << "'" << endl;
            usage(exename);
            return 3;
        }
    }
    
    /******** Parsage ********/

    

    if(arg_file)
    {
        if(arg_file->is_open())
            arg_in = arg_file;
        else
        {
            cerr << "unable to open input file" << endl;
            return 1;
        }
    }
    
    FrBParser parser;
    FrBParser::Tree * tree = parser.classList();
    
    FrBResolveEnvironment renv(tree);
    
    
#ifndef DEBUG_NO_CATCH
    try
    {
#endif
    
        addClass(tree, FrBObject::initClass());
        addClass(tree, FrBNull::initClass());
        addClass(tree, FrBVoid::initClass());
        addClass(tree, FrBBool::initClass());
        addClass(tree, FrBInt::initClass());
        addClass(tree, FrBString::initClass());
        addClass(tree, FrBClassWrapper::initClass());
        addClass(tree, FrBFunctionWrapper::initClass());
        addClass(tree, FrBEventWrapper::getCppClass());
        addClass(tree, FrBDebug::initClass());
        
	/* Alias EN */
        addClassAlias(tree, FrBBool::getCppClass(), "Boolean");
        addClassAlias(tree, FrBInt::getCppClass(), "Integer");


	/* Alias FR */
        addClassAlias(tree, FrBInt::getCppClass(), "Entier");
        addClassAlias(tree, FrBBool::getCppClass(), "Booleen");
        addClassAlias(tree, FrBString::getCppClass(), "Chaine");
        addClassAlias(tree, FrBNull::getCppClass(), "Rien");
        addClassAlias(tree, FrBEventWrapper::getCppClass(), "Evenement");
        
        
        bool success = parser.parse(*arg_in);
        
        if(arg_file)
        {
            arg_file->close();
            delete arg_file;
        }
          
        if(!success)
            return 2;

	FrBKeywords::setCurrentSet(arg_tree_set);
            
        if(args_switch[SHOW_PARSE_TREE])
            parser.printTree(cout, !args_switch[SHOW_ALL_CLASSES]);
            
        /******** Amélioration de l'arbre ********/

        parser.resolveAndCheckTree(renv);
        
        if(args_switch[SHOW_IMPROVED_TREE])
            parser.printTree(cout, !args_switch[SHOW_ALL_CLASSES]);
        
        /******** Interprétation/exécution ********/
            
        FrBMemory memory(100 * FrBMemory::B, 10 * FrBMemory::B, 2, 0);
        FrBExecutionEnvironment env(&memory, &renv);

        env.addGarbagedObject(FrBNull::nullValue());
        env.addGarbagedObject(FrBBool::trueValue());
        env.addGarbagedObject(FrBBool::falseValue());

        FrBClass * main = renv.getClassFromPath(arg_main_class);

	/*
	//cout << "######### Template test. begin #########\n";
	FrBTemplateSpecializationEnvironment tenv;
	FrBClass * cpy = main->specializeTemplate(tenv);
	//addClassAlias(tree, cpy, "MainCopy");
	//cout << "orginal main=" << main << endl << *main << endl;
	//cout << "main copy=" << cpy << endl << *cpy << endl;
	//cout << "######### Template test. end #########\n";
	main = cpy;
	*/

        if(args_switch[EXEC])
        {
    
	    /****** Shared initialization *******/
	    cout << "Shared initialization...\n";
	    sharedInit(tree, env);

            cout << "Call to " << arg_main_class << "::" << arg_main_function;
            
            if(arg_int_param > -1) 
                cout << "(Int = " << arg_int_param << ")...\n";
            else
                cout << "()...\n";

            FrBBaseObjectList args;
            
            FrBInt param0(arg_int_param);
            
            if(arg_int_param > -1)
                args.push_back(&param0);
            
            main->findFunction(arg_main_function, args)->execute(env, 0, args);

            if(args_switch[SHOW_MEM])
            {
                cout << "Mem after call:\n";
                memory.print(1);
            }
            
            if(args_switch[SHOW_STACK])
                env.stack().print();
        }
        
#ifndef DEBUG_NO_CATCH
    }
    catch(const FrBResolveException &e)
    {
        cerr << " resolve error: '" << typeid(e).name() <<  "' throwed" << endl;
        cerr << '\t' << e << endl;
    }
    catch(const FrBExecutionException &e)
    {
        cerr << "execution error: '" << typeid(e).name() <<  "' throwed" << endl;
        cerr << '\t' << e << endl;
    }
    catch(const FrBException &e)
    {
        cerr << "error: '" << typeid(e).name() <<  "' throwed" << endl;
        cerr << '\t' << e << endl;
    }
    catch(...)
    {
        cerr << "unknown exception throwed" << endl;
    }
#endif
      

    return 0;

}


