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


#ifndef FRBFUNCTION_H
#define FRBFUNCTION_H

#include "frbstat.h"
#include "frbexceptions.h"
#include "../common/assert.h"

#include "frbexprlist.h"
#include "frbmember.h"
#include "frbconstants.h"


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
    bool _shared;
    bool _const; 
    bool _abstract;
    //TODO, const=true => Me est const dans cette foction
    String _name;
    
    //FrBClassList _argsType;
    const FrBClass * _returnType;
    
    /* the first parameter that is optional */
    int _firstOptional;
    
    /* the last parameter is a ParamArray */
    bool _paramArray;
    
    scope_t  _scope;
    
public:

    enum { SHARED = 0 };

    typedef enum { NO_MATCH = 0,
                   MATCH_WITH_CONV,
                   MATCH_WITH_PARAM_ARRAY,
                   MATCH_WITH_OPT,
                   MATCH } match_t;
 
    inline FrBFunction() : _sub(false), _shared(false), _const(false), _abstract(false), _firstOptional(-1),
                           _paramArray(false), _scope(SC_PUBLIC)  {};
                             
    virtual ~FrBFunction() {}
    virtual std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    inline void setSub(bool v) { _sub = v; }
    inline bool sub() const { return _sub; }
    
    inline void setShared(bool v) { _shared = v; }
    inline bool shared() const { return _shared; }
    
    inline void setConst(bool v) { _const = v; }
    inline bool isConst() const { return _const; }
    
    inline void setAbstract(bool v) { _abstract = v; }
    inline bool abstract() const { return _abstract; }
    
    inline scope_t scope() const { return _scope; }
    inline void setScope(scope_t v) { _scope = v; }
    
    inline void setParamArrayUsed(bool v) { _paramArray = v; }
    inline bool paramArrayUsed() const { return _paramArray; }
    
    inline void setFirstOptionalParameter(int v) { _firstOptional = v; }
    inline int firstOptionalParameter() const { return _firstOptional; }
    
    virtual const FrBClass * parameterType(int index) const = 0;
    virtual bool parameterByVal(int index) const = 0;
    virtual int parameterCount() const = 0;
    
    inline void setReturnType(const FrBClass * t) { _returnType = t; }
    inline const FrBClass * returnType() const { return _sub ? 0 : _returnType; }
    
    inline void setName(const String& n) { _name = n; }
    inline const String& name() const { return _name; }
    
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

    struct Param
    {
        FrBTypeExpr * type;
        bool byval;
        FrBExpr * init;
        
        Param()
        : type(0), byval(false), init(0) {}
        
        Param(FrBTypeExpr * t, bool b, FrBExpr * i)
         : type(t), byval(b), init(i) {}
        
    };
    
    typedef std::map<const String, int> NameParamList;
    typedef std::vector<Param>          ParamList;
    
    typedef int VarID;
    
    typedef std::map<const String, VarID> NameVarList;
    typedef std::vector<FrBTypeExpr*> VarList;
    
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
    
    /** add a local var */
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


