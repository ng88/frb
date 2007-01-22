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

#include "frbexpr.h"
#include "frbfunction.h"
#include "frbclass.h"
#include "../common/assert.h"
#include "frbexecutionenvironment.h"
#include "frbkeywords.h"
#include "frbbuiltinclasses.h"

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

const FrBClass * FrBFunction::returnType() const
{
    return _sub ? FrBVoid::getCppClass() : _returnType;
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
        for(int i = 0; i < count - 1; ++i)  \
        { \
            if( (a = parameterType(i)) != (b = ARGS) ) \
            { \
                if(b->isCompatibleWith(a)) \
                    full_match = false; \
                else \
                    return NO_MATCH; \
            } \
        } \
         \
        for(int i = count - 1; i < pcount; ++i) \
        { \
            if( (a = parameterType(count - 1)) != (b = ARGS) ) \
            { \
                if(b->isCompatibleWith(a)) \
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
        for(int i = 0; i < pcount; ++i) \
        { \
            if( (a = parameterType(i)) != (b = ARGS) ) \
            { \
                if(b->isCompatibleWith(a)) \
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
    
    frb_assert(sub() || (!sub() && returnType()));
    
    String str_indent(indent, '\t');
    
    stream  << str_indent << FrBKeywords::scopeToString(scope()) << ' '
	    << FrBKeywords::sharedToString(shared()) << ' '
	    << FrBKeywords::constToString(isConst()) << ' '
	    << FrBKeywords::abstractToString(abstract()) << ' '
	    << FrBKeywords::fnToString(sub()) << ' '
	    << name() << '(';
            
    int pcount = parameterCount();
    for(int i = 0; i < pcount; ++i)
    {
      if(firstOptionalParameter() > -1 && i >= firstOptionalParameter())
	stream << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_OPTIONAL) << ' ';
            
      if(paramArrayUsed() && i == parameterCount() - 1)
	stream << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_PARAMARRAY) << ' ';

      stream << FrBKeywords::byvalToString(parameterByVal(i)) << ' '
	     << 'p' << i << ' '
	     << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_AS)
	     << ' ' << parameterType(i)->name();
      
      if(i < pcount - 1)
	stream << ", ";
            
    }
    stream << ") ";
            
    if(!sub())        
      stream  << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_AS) << ' '
	      << returnType()->name();
            
    return stream << endl;
}



/*           FrBCodeFunction            */

/*         Param        */

FrBCodeFunction::Param::~Param()
{
    //done in ~FrBCodeFunction();
}

FrBCodeFunction::Param::Param()
: type(0), byval(false), init(0)
{
}

FrBCodeFunction::Param::Param(FrBTypeExpr * t, bool b, FrBExpr * i)
  : type(t), byval(b), init(i) 
{
}



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
	delete *it;

    _stats.clear();

    for(ParamList::iterator it = _param.begin(); it != _param.end(); ++it)
    {

	delete it->type;

	if(it->init)
	    delete it->init;
    }
    _param.clear();


    //_var element disposal is done in FrBDeclareStatement
    _var.clear();
	

    if(_unresolvedRetType)
	delete _unresolvedRetType;

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

    if(e.stack().overflow())
        throw FrBStackOverflowException();
    
    int var_count = args.size() + localVarCount() + 1;
     
    /* empile les paramètres */
    e.stack().push(args);
        
    /* empile me */
    e.stack().push(me);
    
    /* empile une place les variables locales */
    e.stack().push_empty(localVarCount());
    
    try
    {
      for(FrBStatementlist::const_iterator it = _stats.begin(); it != _stats.end(); ++it)
        (*it)->execute(e);
    }
    catch(FrBReturnException ex)
    {
      e.stack().pop(var_count);
      return ex.value();
    }   
    
    e.stack().pop(var_count);
    
    return FrBVoid::voidValue();
    
}

bool FrBCodeFunction::allPathContainsAReturn() const
{
  for(FrBStatementlist::const_iterator it = _stats.begin(); it != _stats.end(); ++it)
    if((*it)->allPathContainsAReturn())
      return true;

  return false;
}


void FrBCodeFunction::resolvePrototype(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    if(!sub())
    {
        frb_assert(_unresolvedRetType);
        
        _unresolvedRetType->resolveAndCheck(e);
        setReturnType(_unresolvedRetType->getClass());
        
        frb_assert(returnType());
    }

    for(ParamList::iterator it = _param.begin(); it != _param.end(); ++it)
        it->type->resolveAndCheck(e);
}

void FrBCodeFunction::resolveAndCheck(FrBResolveEnvironment& e) throw (FrBResolveException)
{
    for(FrBStatementlist::iterator it = _stats.begin(); it != _stats.end(); ++it)
    {
        frb_assert(*it);
        (*it)->resolveAndCheck(e);
    }
}

std::ostream& FrBCodeFunction::put(std::ostream& stream, int indent) const
{
    using namespace std;
    
    frb_assert(sub() || (!sub() && _unresolvedRetType));
    
    String str_indent(indent, '\t');
    
    stream  << str_indent << FrBKeywords::scopeToString(scope()) << ' '
	    << FrBKeywords::sharedToString(shared()) << ' '
	    << FrBKeywords::constToString(isConst()) << ' '
	    << FrBKeywords::abstractToString(abstract()) << ' '
	    << FrBKeywords::fnToString(sub()) << ' '
	    << name() << '(';
            
    int pcount = parameterCount();
    for(int i = 0; i < pcount; ++i)
    {
      if(firstOptionalParameter() > -1 && i >= firstOptionalParameter())
	stream << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_OPTIONAL) << ' ';
            
      if(paramArrayUsed() && i == parameterCount() - 1)
	stream << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_PARAMARRAY) << ' ';

      stream << FrBKeywords::byvalToString(parameterByVal(i)) << ' '
	     << 'p' << i << ' '
	     << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_AS)
	     << ' ' << *getURParam(i);
      
      if(i < pcount - 1)
	stream << ", ";
            
    }
    
    stream << ") ";       
    
    if(!sub())        
      stream  << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_AS) << ' '
	      << *_unresolvedRetType;

    stream << endl;
            
    indent++;

    for(FrBStatementlist::const_iterator it = _stats.begin(); it != _stats.end(); ++it)
    {
      stream << str_indent << '\t';
      (*it)->put(stream, indent);
      stream << endl;
    }
    
    return stream << str_indent
		  << FrBKeywords::getKeyword(FrBKeywords::FRB_KW_END)
		  << ' ' << FrBKeywords::fnToString(sub()) << endl;
}



