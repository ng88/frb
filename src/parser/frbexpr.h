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


#ifndef FRBEXPR_H
#define FRBEXPR_H

#include "frbbuiltinclasses.h"
#include "frbexceptions.h"
#include "frbexecutionenvironment.h"

class FrBCodeFunction;

typedef std::map<const String, unsigned int> SymbolTable;

class FrBExpr
{
public:
    virtual ~FrBExpr() {}
    
    /** Resolve unresolved identifier (class, function) and check type compatibility */
    virtual void resolveAndCheck() throw (FrBResolveException) {}
    
    /** Get type of expression (can be called ONLY IF resolve() was called before) */
    virtual const FrBClass* getClass() const = 0;
    
    /** Evaluate expression (can be called ONLY IF resolve() was called before) */
    virtual FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException) = 0;
    
    /** Print expression on stream */
    virtual std::ostream& put(std::ostream& stream) const = 0;
};

std::ostream& operator<<(std::ostream& s, const FrBExpr& expr);

typedef std::vector<FrBExpr*> FrBExprList;


/** Local var (for localvar, parameters & me) */
class FrBLocalVarExpr : public FrBExpr
{
private:
    const FrBClass *   _type;
    int               _varid;
    
public:
    FrBLocalVarExpr(const FrBClass * t, int varid);
    ~FrBLocalVarExpr();
    
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    std::ostream& put(std::ostream& stream) const;
};

/** Member operator (.) */
class FrBMemberOpExpr : public FrBExpr
{
    FrBExpr     *_lhs;
    FrBExpr   *_rhs;
    
public:
    FrBMemberOpExpr(FrBExpr* lhs, FrBExpr* rhs);
    ~FrBMemberOpExpr();
    
    void resolveAndCheck() throw (FrBResolveException);
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    std::ostream& put(std::ostream& stream) const;  
};

class FrBBinOpExpr : public FrBExpr
{
private:
    FrBExpr     *_rhs;
    FrBExpr     *_lhs;
    int          _op;
    FrBFunction *_fn;
    
public:
    FrBBinOpExpr(FrBExpr* lhs, FrBExpr* rhs, int op) throw (FrBFunctionNotFoundException);
    ~FrBBinOpExpr();
    
    void resolveAndCheck() throw (FrBResolveException);
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    std::ostream& put(std::ostream& stream) const;    
};

//TODO recuperer le typ des expr afin de pouvoir resoudre la bonne surcharge lors de la construction de l'arbre
//TODO afin d'ajouter/enlever les operation de conversion

/*class FrBNewExpr : public FrBExpr
{
private:
    FrBExprList * _args;
    FrBFunction * _ctor;
    
public:
    FrBNewExpr( FrBExprList * _args);
    ~FrBNewExpr();
    FrBBaseObject* eval() const throw (FrBEvaluationException);
    std::ostream& put(std::ostream& stream) const;    
};*/

template<class literal_type>
class FrBLiteralExpr : public FrBExpr
{
private:
    //FrBCppObject* _pvalue;
    literal_type  _value;
    
public:
    FrBLiteralExpr(const literal_type& v)
    {
        //_pvalue = new FrBPrimitive<literal_type>(v);
        _value = v;
        //TODO pvalue ne doit pas etre modifi�, doit etre de type FrBConstInt
    }
    
    ~FrBLiteralExpr()
    {
        //FrBBoxing::dispose_obj(_pvalue);
        //on ne delete pas _pvalue
        //on devrait pourtant, enfin on vera ca plus tard
    }

    
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
    {
        FrBCppObject* o = new FrBPrimitive<literal_type>(_value);
        
        e.addGarbagedObject(o);
        
        return o;
    }
    
    const FrBClass* getClass() const
    {
        return  FrBPrimitive<literal_type>::getCppClass();
    }
    
    std::ostream& put(std::ostream& stream) const
    {
        //return stream << "<val=" << _value << ">";
        return stream << "<literal_val>";
    }
};

typedef FrBLiteralExpr<int>        FrBIntExpr;
// typedef FrBLiteralExpr<double>          FrBDoubleExpr;
// typedef FrBLiteralExpr<float>           FrBSingleExpr;
// typedef FrBLiteralExpr<short int>       FrBShortIntExpr;
// typedef FrBLiteralExpr<long int>        FrBLongIntExpr;
typedef FrBLiteralExpr<String>          FrBStringExpr;
// typedef FrBLiteralExpr<char>            FrBCharExpr;


#endif


