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

#include "frbexpr.h"
#include "frbfunction.h"
#include "frbclass.h"
#include "../common/assert.h"
#include "frbexecutionenvironment.h"

std::ostream& operator<<(std::ostream& s, const FrBFunction& fn)
{
    return fn.put(s);
}

/*          FrBFunction             */


FrBBaseObject * FrBFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me) const
    throw (FrBExecutionException)
{
    FrBBaseObjectList args;
    return execute(e, me, args);
}

FrBBaseObject * FrBFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me, FrBBaseObject * arg0) const
        throw (FrBExecutionException)
{
    FrBBaseObjectList args;
    args.push_back(arg0);
    return execute(e, me, args);
}

FrBBaseObject * FrBFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me,
                                        FrBBaseObject * arg0, FrBBaseObject * arg1) const
        throw (FrBExecutionException)
{
    FrBBaseObjectList args;
    args.push_back(arg0);
    args.push_back(arg1);
    return execute(e, me, args);
}

// Pas tres propre mais j'ai pas mieux pour le moment :
#define COMMON_MATCH_PARAM_BODY(ARGS) \
   int count = parameterCount(); \
    int pcount = args.size(); \
     \
    bool full_match = true; \
     \
    const FrBClass * a; \
    const FrBClass * b; \
     \
    if(_paramArray) \
    { \
        if(pcount < count - 1) \
            return NO_MATCH; \
             \
        for(int i = 0; i < count - 1; ++i) /* fn pour ca */ \
        { \
            if( (a = parameterType(i)) != (b = ARGS) ) \
            { \
                if(FrBClass::areCompatibles(a, b)) \
                    full_match = false; \
                else \
                    return NO_MATCH; \
            } \
        } \
         \
        for(int i = count - 1; i < pcount; ++i)/* fn pour ca */ \
        { \
            if( (a = parameterType(count - 1)) != (b = ARGS) ) \
            { \
                if(FrBClass::areCompatibles(a, b)) \
                    full_match = false; \
                else \
                    return NO_MATCH; \
            } \
        } \
        \
        return full_match ? MATCH_WITH_PARAM_ARRAY : MATCH_WITH_CONV;  \
    } \
    else \
    { \
        if(pcount > count) \
            return NO_MATCH; \
             \
        bool opt_match = (_firstOptional > -1); \
         \
        if(!opt_match && pcount != count ) \
            return NO_MATCH; \
             \
        if(opt_match && pcount < _firstOptional ) \
            return NO_MATCH; \
          \
        for(int i = 0; i < pcount; ++i)/* fn pour ca */ \
        { \
            if( (a = parameterType(i)) != (b = ARGS) ) \
            { \
                if(FrBClass::areCompatibles(a, b)) \
                    full_match = false; \
                else \
                    return NO_MATCH; \
            } \
        } \
         \
        return full_match ? (opt_match ? MATCH_WITH_OPT : MATCH) : MATCH_WITH_CONV; \
    }
   

FrBFunction::match_t FrBFunction::matchParameters(const FrBConstClassList& args)
{
    COMMON_MATCH_PARAM_BODY((args[i]));
}


FrBFunction::match_t FrBFunction::matchParameters(const FrBBaseObjectList& args)
{
    COMMON_MATCH_PARAM_BODY((args[i]->getClass()));
}


FrBFunction::match_t FrBFunction::matchParameters(const FrBExprList& args)
{
    COMMON_MATCH_PARAM_BODY((args[i]->getClass()));
}



std::ostream& FrBFunction::put(std::ostream& stream, int indent) const
{
    using namespace std;
    
    String str_indent(indent, '\t');
    

    stream  << str_indent << "\t    Function " << name() << endl
            << str_indent << "\t\t- Scope=" << scope() << endl
            << str_indent << "\t\t- Shared=" << shared() << endl
            << str_indent << "\t\t- Sub=" << sub() << endl
            << str_indent << "\t\t- Const=" << isConst() << endl
            << str_indent << "\t\t- Abstract=" << abstract() << endl
            << str_indent << "\t\t- Parameters:" << endl;
            
    for(int i = 0; i < parameterCount(); ++i)
    {
        stream << str_indent << "\t\t\t* " << (parameterByVal(i) ? "byval " : "byref ")
                                           << parameterType(i)->name();
        
        if(firstOptionalParameter() > -1 && i >= firstOptionalParameter())
            stream << " (optional)";
            
        if(paramArrayUsed() && i == parameterCount() - 1)
            stream << " (param_array)";
            
        stream << endl;
    }
            
    if(!sub())        
        stream  << str_indent << "\t\t- Return type=" << returnType()->name() << endl;
            
    return stream;
}



/*           FrBCodeFunction            */
FrBCodeFunction::FrBCodeFunction()
{
    _sub = false;
    _shared = false;
    _const = false;
    _unresolvedRetType = 0;
}


FrBCodeFunction::~FrBCodeFunction()
{
    for(FrBStatementlist::iterator it = _stats.begin(); it != _stats.end(); ++it)
        delete(*it);
}

const FrBClass * FrBCodeFunction::parameterType(int index) const
{
    frb_assert2(index < parameterCount(), "FrBCodeFunction::parameterType(int) / index out of bounds");

    return _param[index].type->getClass();
}

int FrBCodeFunction::parameterCount() const
{
    return _param.size();
}

bool FrBCodeFunction::parameterByVal(int index) const
{
    frb_assert2(index < parameterCount(), "FrBCodeFunction::parameterByVal(int) / index out of bounds");
    return _param[index].byval;
}


FrBBaseObject * FrBCodeFunction::execute(FrBExecutionEnvironment& e, FrBBaseObject * me,
                                                const FrBBaseObjectList& args) const
    throw (FrBExecutionException)
{

    
    int var_count = args.size() + localVarCount() + 2;
     
    /* empile les paramètres */
    e.stack().push(args);
        
    /* empile me */
    e.stack().push(me);
    
    /* empile une place pr le retour et les variables locales */
    e.stack().push_empty(localVarCount() + 1);
    
    for(FrBStatementlist::const_iterator it = _stats.begin(); it != _stats.end(); ++it)
        (*it)->execute(e);
       

    FrBBaseObject * ret = e.stack().getTopValue(localVarCount());

    e.stack().pop(var_count);
    
    return _sub ? 0 : ret;
    
}

void FrBCodeFunction::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    if(!sub())
    {
        frb_assert(_unresolvedRetType);
        _unresolvedRetType->resolveAndCheck(e);
        setReturnType(_unresolvedRetType->getClass());
    }
    
    for(ParamList::iterator it = _param.begin(); it != _param.end(); ++it)
        it->type->resolveAndCheck(e);
    
        //TODO ca fait doublon car deja fais par declare stat
        //doit on garder ca ?
    /*for(VarList::iterator it = _var.begin(); it != _var.end(); ++it)
        it->resolveAndCheck(e);*/
    
        //TODO copie de e avec des infos qui vont bien (fonction, classe, outer classe)
        
    for(FrBStatementlist::iterator it = _stats.begin(); it != _stats.end(); ++it)
    {
        frb_assert((*it));
        (*it)->resolveAndCheck(e);
    }
}

std::ostream& FrBCodeFunction::put(std::ostream& stream, int indent) const
{
    using namespace std;
    
    String str_indent(indent, '\t');


    stream  << str_indent << "\t    Function " << name() << endl
            << str_indent << "\t\t- Scope=" << scope() << endl
            << str_indent << "\t\t- Shared=" << shared() << endl
            << str_indent << "\t\t- Sub=" << sub() << endl
            << str_indent << "\t\t- Const=" << isConst() << endl
            << str_indent << "\t\t- Abstract=" << abstract() << endl
            << str_indent << "\t\t- Parameters:" << endl;
            
    for(int i = 0; i < parameterCount(); ++i)
    {
        stream << str_indent << "\t\t\t* " << (parameterByVal(i) ? "byval " : "byref ")
                                           << *getURParam(i);
        
        if(firstOptionalParameter() > -1 && i >= firstOptionalParameter())
            stream << " (optional)";
            
        if(paramArrayUsed() && i == parameterCount() - 1)
            stream << " (param_array)";
            
        stream << endl;
    }
            
    if(!sub())        
        stream  << str_indent << "\t\t- Return type=" << *_unresolvedRetType << endl;
    

    
    stream  << str_indent << "\t\t- Local vars:" << endl;
    
    for(int i = 0; i < localVarCount(); ++i)
    {
        stream << str_indent << "\t\t\t* " << i << " (" << *getLocalVar(i) << ")" << endl;
    }
            
    stream  << str_indent << "\t\t- Statements:" << endl;
    
    for(FrBStatementlist::const_iterator it = _stats.begin(); it != _stats.end(); ++it)
        stream << str_indent << "\t\t\t*stat> " << **it << endl;
    
    return stream;
}



