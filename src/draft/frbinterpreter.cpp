

#include "frbinterpreter.h"
#include <iostream>

#define debug(x) { DebugInfos db = { "", x }; EMIT_EVENT(Debug, db); }
#define error(l, x, c) { ErrorInfos err = { l, 0, 0, x, c }; EMIT_EVENT(Error, err); }

FrBInterpreter::FrBInterpreter()
{
    CONNECT_EVENT( Error, NULL );
    CONNECT_EVENT( Debug, NULL );
    
}

FrBInterpreter::~FrBInterpreter()
{
}    

void FrBInterpreter::addCode(const String& code)
{
    _code += code;
}

void FrBInterpreter::addObject(FrBBaseObject * obj)
{
    FRB_UNUSED(obj);
}




bool FrBInterpreter::execute()
{   
    
    bool ret = _parser.parse(_code);
    
    return ret;
}

bool FrBInterpreter::execute(const String& code, ErrorCallback fn)
{
    FrBInterpreter tmp;
    tmp.addCode(code);
    tmp.CONNECT_EVENT(Error, fn);
    return tmp.execute();
}



