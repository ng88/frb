
#include <iostream>
#include "frbparser.h"

using namespace std;

int main(int argc, char ** argv)
{

    FrBParser parser;

    try
    {
    
        (*(parser.classList()))["Int"] = FrBInt::initClass();
        (*(parser.classList()))["Debug"] = FrBDebug::initClass();
        
        
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
