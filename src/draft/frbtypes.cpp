
#include "frbtypes.h"

FrBTest::FrBTest()
{
}

void FrBTest::fn_test(FrBString * arg1, FrBString * arg2, FrBInt * arg3)
{
    std::cout << "FrBTest::fn_test()\n";
    std::cout << " Class " << this << "\n";
    std::cout << " <- " << arg1 << "\n";
    std::cout << " <- " << arg2 << "\n";
    std::cout << " <- " << arg3 << "\n";
}

void FrBConsole::write(FrBString * str)
{
}

void FrBConsole::write(FrBInt * str)
{
}

FrBInt* FrBConsole::readInt()
{
    FrBInt* ret = dynamic_cast<FrBInt*>(ObjectFactory::newObject("Int"));
    
    return ret;
}

FrBString* FrBConsole::readString()
{
    FrBString* ret = dynamic_cast<FrBString*>(ObjectFactory::newObject("String"));
    
    return ret;
}

FRB_TYPE_DEC(FrBObject);
FRB_TYPE_DEC(FrBString);
FRB_TYPE_DEC(FrBInt);
FRB_TYPE_DEC(FrBTest);
FRB_TYPE_DEC(FrBConsole);
FRB_TYPE_DEC(FrBFunction);
FRB_TYPE_DEC(FrBUserClass);




