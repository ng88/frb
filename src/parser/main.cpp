
#include <iostream>
#include "frbparser.h"

using namespace std;

inline void addClass(FrBParser::Tree * tree, FrBClass * c) { (*tree)[c->name()] = c; }

int main(int argc, char ** argv)
{

    enum { SHOW_TREE = 0, SHOW_MEM, SWITCH_COUNT };
    bool args_switch[SWITCH_COUNT];
    
    args_switch[SHOW_TREE] = true;
    args_switch[SHOW_MEM] = true;
    
    char * arg_main_class = "Main";
    char * arg_main_function = "main";
    
    for(int i = 0; i < argc; ++i)
    {
        if(!strcmp(argv[i], "--no-tree"))
            args_switch[SHOW_TREE] = false;
        else if(!strcmp(argv[i], "--no-mem"))
            args_switch[SHOW_MEM] = false;
        else if(!strcmp(argv[i], "--main-class") && i + 1 < argc)
            arg_main_class = argv[++i];
        else if(!strcmp(argv[i], "--main-function") && i + 1 < argc)
            arg_main_function = argv[++i];  
    }

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
    
        FrBClass * main = FrBClass::getClassFromString(arg_main_class);
    
        cout << "Call to " << arg_main_class << "::" << arg_main_function << "()...\n";
        
        FrBBaseObjectList args;
        main->executeFunction(arg_main_function, 0, args);
    
        if(args_switch[SHOW_MEM])
        {
            cout << "Mem after " << arg_main_class << "::" << arg_main_function << "() call:\n";
            FrBMemory::getMemory()->print(1);
        }
        
        FrBMemory::destroyMemory();
    }
    catch(const FrBExecutionException &e)
    {
        cerr << '`' << typeid(e).name() <<  "' throwed\n";
        cerr << '\t' << e << '\n';
    }
    
      
    return 0;

}
