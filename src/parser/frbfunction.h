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


#ifndef FRBFUNCTION_H
#define FRBFUNCTION_H

#include "frbstat.h"
#include "frbexceptions.h"
#include "../common/assert.h"

#include "frbexprlist.h"
#include "frbmember.h"


class FrBClass;
class FrBFunction;
typedef std::vector<FrBClass*> FrBClassList;
typedef std::vector<const FrBClass*> FrBConstClassList;
typedef std::map<const String, FrBClass*> FrBClassMap;
typedef std::vector<FrBFunction*> FrBFunctionList;

class FrBFunction : public FrBMember /* a frb function */
{
protected:
    bool _sub;
    bool _abstract;
    //TODO, const=true => Me est const dans cette foction
    
    //FrBClassList _argsType;
    const FrBClass * _returnType;
    
    /* the first parameter that is optional */
    int _firstOptional;
    
    /* the last parameter is a ParamArray */
    bool _paramArray;
    
    scope_t  _scope;
    
public:

    //enum { SHARED = 0 };

    typedef enum { NO_MATCH = 0,
                   MATCH_WITH_CONV,
                   MATCH_WITH_PARAM_ARRAY,
                   MATCH_WITH_OPT,
                   MATCH } match_t;
 
    inline FrBFunction() : _sub(false), _abstract(false), _firstOptional(-1),
                           _paramArray(false)  {};
                             
    virtual ~FrBFunction() {}
    virtual std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    inline void setSub(bool v) { _sub = v; }
    inline bool sub() const { return _sub; }
    
    inline void setAbstract(bool v) { _abstract = v; }
    inline bool abstract() const { return _abstract; }
  
    inline void setParamArrayUsed(bool v) { _paramArray = v; }
    inline bool paramArrayUsed() const { return _paramArray; }
    
    inline void setFirstOptionalParameter(int v) { _firstOptional = v; }
    inline int firstOptionalParameter() const { return _firstOptional; }
    
    virtual const FrBClass * parameterType(int index) const = 0;
    virtual bool parameterByVal(int index) const = 0;
    virtual int parameterCount() const = 0;
    
    inline void setReturnType(const FrBClass * t) { _returnType = t; }
    const FrBClass * returnType() const;
    
    virtual FrBBaseObject * execute(FrBExecutionEnvironment& env, FrBBaseObject * me) const
        throw (FrBExecutionException);
    virtual FrBBaseObject * execute(FrBExecutionEnvironment& env, FrBBaseObject * me, FrBBaseObject * arg0) const
        throw (FrBExecutionException);
    virtual FrBBaseObject * execute(FrBExecutionEnvironment& env, FrBBaseObject * me, FrBBaseObject * arg0, 
                         FrBBaseObject * arg1) const
        throw (FrBExecutionException);
    virtual FrBBaseObject * execute(FrBExecutionEnvironment& env, FrBBaseObject * me,
                        const FrBBaseObjectList& args) const
        throw (FrBExecutionException) = 0;
    
    match_t matchParameters(const FrBBaseObjectList& args);
    match_t matchParameters(const FrBConstClassList& args);
    match_t matchParameters(const FrBExprList& args);
    

    template<class It, class ArgContainer>    
    static FrBFunction* findOverload(const String& name, It begin, const It& end, const ArgContainer& args)
        throw (FrBFunctionNotFoundException)
    {
        FrBFunction * bestMatch = 0;
        match_t matchDegree = NO_MATCH;
        
        for(; begin != end && matchDegree != MATCH; ++begin)
        {
            match_t m = (*begin)->matchParameters(args);
    
            if( m >= matchDegree)
            {
                matchDegree = m;
                bestMatch = *begin;
            }
        }
        
        if(matchDegree == NO_MATCH)
            throw FrBFunctionNotFoundException(name, args);
            
        return bestMatch;
    }
    
    /** Used in type resolution */
    virtual void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException) {};
    virtual void resolvePrototype(FrBResolveEnvironment&) throw (FrBResolveException) {};
        
};

std::ostream& operator<<(std::ostream& s, const FrBFunction& fn);


class FrBCodeFunction : public FrBFunction, public FrBStatementBlock
{
public:

    /** Represents a function parameter
      */
    struct Param
    {
        FrBTypeExpr * type;
        bool byval;
        FrBExpr * init;
        
        Param();
        ~Param();
        Param(FrBTypeExpr * t, bool b, FrBExpr * i);
        
    };
    
    typedef std::map<const String, int> NameParamList;
    typedef std::vector<Param>          ParamList;
    
    typedef int VarID;
    
    typedef std::map<const String, VarID> NameVarList;
    typedef std::vector<FrBTypeExpr*>     VarList;
    
protected:
    
    NameParamList        _paramName;
    ParamList            _param;
    
    NameVarList          _varName;
    VarList              _var;
    
    FrBTypeExpr*         _unresolvedRetType;

    
public:
    FrBCodeFunction();
    ~FrBCodeFunction();
    
    //TODO verif nom des params (doublons)
    inline void addParam(const String& name, FrBTypeExpr* v, bool byval, FrBExpr * init = 0);
    
    /** Return the param index if found or -1 if not found */
    inline int getParam(const String& name) const;
    
    /** Display on stream a string representation of this */
    std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    const FrBClass * parameterType(int index) const;
    int parameterCount() const;
    bool parameterByVal(int index) const;
    
    FrBBaseObject * execute(FrBExecutionEnvironment& env, FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);
        
    /* local var handling functions */
    
    /** Add a local var named 'name' of type 'type'
      * Note: 'type' will not be resolved or deleted in this class,
      *       because that will be done by the FrBDeclareStatement class
      */
    inline void addLocalVar(String name, FrBTypeExpr* type);
    
    /** Return a pointer to a Param struct if found or 0 if not found */
    inline VarID findLocalVar(String name) const;
    
    inline FrBTypeExpr * getLocalVar(VarID id) const;
    
    inline int localVarCount() const;
    
    /** Set unresolved return type */
    inline void setURReturnType(FrBTypeExpr*);
    
    /** Get unresolved param */
    inline FrBTypeExpr * getURParam(int index) const;
 
    bool allPathContainsAReturn() const;
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void resolvePrototype(FrBResolveEnvironment&) throw (FrBResolveException);
    
};



class FrBProperty
{
private:
    FrBFunction * _get;
    FrBFunction * _set;
};

typedef std::vector<FrBCodeFunction*> FrBCodeFnList;
typedef std::stack<FrBCodeFunction*> FrBCodeFunctionStack;


















/* Inlined */

inline void FrBCodeFunction::addParam(const String& name, FrBTypeExpr* v, bool byval, FrBExpr * init)
{
    _paramName[name] = _param.size();
    _param.push_back(Param(v, byval, init));
}

inline int FrBCodeFunction::getParam(const String& name) const
{
    NameParamList::const_iterator it = _paramName.find(name);
    
    return (it == _paramName.end()) ? -1 : it->second;
}

inline void FrBCodeFunction::addLocalVar(String name, FrBTypeExpr * type)
{
    _varName[name] = _var.size();
    _var.push_back(type);
}


inline FrBCodeFunction::VarID FrBCodeFunction::findLocalVar(String name) const
{
    NameVarList::const_iterator it = _varName.find(name);
    
    return (it == _varName.end()) ? -1 : it->second;
}

inline FrBTypeExpr * FrBCodeFunction::getLocalVar(FrBCodeFunction::VarID id) const
{
    frb_assert(id >= 0 && id < (int)_var.size());
    return _var[id];
}

inline int FrBCodeFunction::localVarCount() const
{
    return _varName.size();
}

inline void FrBCodeFunction::setURReturnType(FrBTypeExpr* t)
{
    frb_assert(sub() || (!sub() && t));
    _unresolvedRetType = t;
}

inline FrBTypeExpr * FrBCodeFunction::getURParam(int index) const
{
    frb_assert2(index < parameterCount(), "FrBCodeFunction::getURParam(int) / index out of bounds");

    return _param[index].type;
}

#endif


