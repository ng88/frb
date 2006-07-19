
#include <iostream>
#include "frbparser.h"

using namespace std;

inline void addClass(FrBParser::Tree * tree, FrBClass * c) { (*tree)[c->name()] = c; }

int main(int argc, char ** argv)
{

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

        parser.printTree();
    
        FrBClass * main = FrBClass::getClassFromString("Main");
    
        FrBBaseObjectList args;
        main->executeFunction("main", 0, args);
    
        cout << "Mem after Main::main() call:\n";
        FrBMemory::getMemory()->print(1);
    
        FrBMemory::destroyMemory();
    }
    catch(const FrBExecutionException &e)
    {
        cerr << '`' << typeid(e).name() <<  "' throwed\n";
        cerr << '\t' << e << '\n';
    }
    
      
    return 0;

}
