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

FrBDeclareStatement::FrBDeclareStatement(int varid, FrBTypeExpr * t, FrBExpr * init_val)
    : _varid(varid), _type(t), _init(init_val)
{

}

void FrBDeclareStatement::resolveAndCheck(const FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _type->resolveAndCheck(e);
    
    if(_init)
    {
        _init->resolveAndCheck(e);
        if(!FrBClass::areCompatibles(_init->getClass(), _type->getClass()))
            throw FrBIncompatibleClassException(_init->getClass(), _type->getClass());
    }
}

void FrBDeclareStatement::execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{

    e.stack().setTopValue(_varid,
                  ((_init == 0) ? _type->getClass()->createInstance(e) :
                                  FrBClass::forceConvert(_init->eval(e), _type->getClass())));
}

std::ostream& FrBDeclareStatement::put(std::ostream& stream) const
{
    stream << "declare <local_var:" << _varid << "> (" << *_type << ')';
    
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

void FrBExprStatement::resolveAndCheck(const FrBResolveEnvironment& e) throw (FrBResolveException)
{
    _expr->resolveAndCheck(e);
}

void FrBExprStatement::execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException)
{
    _expr->eval(e);
}

std::ostream& FrBExprStatement::put(std::ostream& stream) const
{
    return stream << "Eval expression " << *_expr;
}

FrBExprStatement::~FrBExprStatement()
{
    delete _expr;
}



