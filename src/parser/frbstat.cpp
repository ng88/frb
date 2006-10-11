#include "frbstat.h"
#include "frbexpr.h"
#include "frbclass.h"
#include "../common/assert.h"
#include "frbmemory.h"

std::ostream& operator<<(std::ostream& s, const FrBStatement& stat)
{
    return stat.put(s);
}


/*                 FrBDeclareStatement              */

FrBDeclareStatement::FrBDeclareStatement(FrBCodeFunction * fn, int varid, FrBExpr * init_val)
    : _fn(fn), _varid(varid), _init(init_val)
{
}

void FrBDeclareStatement::execute() const throw (FrBExecutionException)
{
    //allocation d'un frbobject + declaration d'une var
    
    //TODO empilage
    
    /*FrBBaseObject * val = (_init == 0) ? _type->createInstance() : FrBClass::convert(_init->eval(), _type);

    FrBMemory::getMemory()->addObject(_name, _type, val);*/
}

std::ostream& FrBDeclareStatement::put(std::ostream& stream) const
{
    stream << "declare <local_var:" << _varid << "> (" << _fn->getLocalVar(_varid)->name() << ')';
    
    if(_init)
        stream << " with init value " << *_init;
    
    return stream;
}

FrBDeclareStatement::~FrBDeclareStatement()
{
}

/*                 FrBExprStatement                */
FrBExprStatement::FrBExprStatement(FrBExpr* expr)
    : _expr(expr)
{
    frb_assert2(expr, "frbparsingtree.h::FrBExprStatement::FrBExprStatement()");
}

void FrBExprStatement::execute() const throw (FrBExecutionException)
{
    _expr->eval();
}

std::ostream& FrBExprStatement::put(std::ostream& stream) const
{
    return stream << "Eval expression " << *_expr;
}

FrBExprStatement::~FrBExprStatement()
{
    delete _expr;
}



