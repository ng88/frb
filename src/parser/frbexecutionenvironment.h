#ifndef FRBEXECUTIONENVIRONMENT_H
#define FRBEXECUTIONENVIRONMENT_H

#include "frbbaseobject.h"

class FrBMemory;

/** This class contains the local execution environment for the current function */
class FrBExecutionEnvironment
{
private:
    FrBBaseObjectList  _vars;
    FrBMemory * _mem;
public:

    /**
      * @param mem Heap memory
      * @param lvar_count Local var count
      */
    FrBExecutionEnvironment(FrBMemory *mem, int lvar_count)
     : _mem(mem)
    {
        _vars.resize(lvar_count);
    }
    
    
    inline FrBBaseObject* getVarValue(int id) { return _vars[id]; }
    inline void setVarValue(int id, FrBBaseObject* v) { _vars[id] = v;; }
    
    inline FrBMemory * memory() { return _mem; }
    
    

};


#endif
