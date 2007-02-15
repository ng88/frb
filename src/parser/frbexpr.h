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


#ifndef FRBEXPR_H
#define FRBEXPR_H

#include "frbbuiltinclasses.h"
#include "frbexceptions.h"
#include "frbexecutionenvironment.h"
#include "frbexprlist.h"

class FrBCodeFunction;

/** FrBExpr represents a general expression

    I) Identifier expression
        There are different kind of identifier expression :
    
            1. We're sure that expr is a type, so used class is FrBUnresolvedTypeExpr("MyClass", current_class)
                for an expr like 'MyClass' or
                FrBUnresolvedTypeExpr("MyClass", current_class, FrBUnresolvedTypeExpr("InnerClass1", current_class))
                for an expr like 'MyClass.InnerClass1'
    
            2. We're sure that expr is Me, so used class is FrBInsideMeExpr if we're in a function block or FrBOutsideMeExpr
                if we're outside (typically in a field initialization)
    
            3. We're sure that expr is Null, so used class is FrBNullExpr

            4. We're sure that expr is a function parameter or a function local var, so used class is FrBLocalVarExpr
    
            5. We don't know yet what expr (it can be a type, a function name, an inner class name, a field...
                but NOT a function parameter, a function local var, Null or Me), so used class is
                FrBUnresolvedIdWithContextExpr
            

    II) Binary operator expression
    III) n-nary operator expression

*/
class FrBExpr
{
public:
    virtual ~FrBExpr() {}
    
    /** Resolve unresolved identifier (class, function) and check type compatibility */
    virtual void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException) {}
    
    /** Get type of expression (can be called ONLY IF resolveAndCheck() was called before) */
    virtual const FrBClass* getClass() const = 0;
    
    /** Evaluate expression (can be called ONLY IF resolveAndCheck() was called before) */
    virtual FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException) = 0;
    
    /** Return true if expr can be assigned, ie if we can write <expr> := something (default is false) */
    virtual bool isAssignable() const;
    
    /** Do assignement */
    virtual void refAssign(FrBExecutionEnvironment&, FrBBaseObject*) const throw (FrBEvaluationException);

    /** Return true if expr is an instance (and not a type) (default is true) */
    virtual bool isInstance() const;
    
    /** Print expression on stream */
    virtual std::ostream& put(std::ostream& stream) const = 0;

};

std::ostream& operator<<(std::ostream& s, const FrBExpr& expr);


/** Represent a type expr, actually, only FrBUnresolvedTypeExpr is a type expr
  * This class was keeped for backward compatibility
  */
class FrBTypeExpr : public FrBExpr
{
};

/** Represent a me expr, actually (ouside or inside)
  */
class FrBMeExpr : public FrBExpr
{
protected:
    bool _nonSharedContext;
public:

    /** sharedContext indicates wheter we are in a shared member definition or not
     */
    FrBMeExpr(bool nonSharedContext);
    bool isInstance();
    virtual ~FrBMeExpr() {}
};

/** hold full type like Module1.Module2.Type, used FOR TYPE ONLY */
class FrBUnresolvedTypeExpr : public FrBTypeExpr
{
protected:
    String                  _name;
    FrBClass *              _type;
    FrBUnresolvedTypeExpr * _context;
    FrBCodeClass *          _currentClass;
    
public:
    FrBUnresolvedTypeExpr(const String& name, FrBCodeClass* current, FrBUnresolvedTypeExpr * ctxt = 0);
    ~FrBUnresolvedTypeExpr();
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    inline FrBClass* getContext() { return _type; }   
    std::ostream& put(std::ostream& stream) const;
    
    inline const String& name() const { return _name; }
};

/** Identifier not yet resolved (ie class name, function, field...)
    current class context is provided, typically it's used in an expression like
    FrBMemberOpExpr(FrBUnresolvedIdWithContextExpr, FrBUnresolvedIdExpr)) <=> unresolved_id_wc.unresolved_id
    or simply
    FrBUnresolvedIdWithContextExpr <=> unresolved_id_wc
*/
class FrBUnresolvedIdWithContextExpr : public FrBExpr
{
protected:

    /** used to eval different kind of identifier */
    class Evaluator
    {
     public:
        virtual ~Evaluator() {}
        virtual const FrBClass* getClass() const = 0;

        /** Return true if the evaluator need the me param to be set for evaluation */
        virtual bool needMe() const = 0;
        
        virtual bool isAssignable() const { return false; }
        virtual void refAssign(FrBExecutionEnvironment&, FrBBaseObject* me, FrBBaseObject* val) const
            throw (FrBEvaluationException) {}

	//virtual void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException) {}

        virtual FrBBaseObject* eval(FrBBaseObject * me, FrBExecutionEnvironment& e) const
            throw (FrBEvaluationException) = 0;

	virtual String name() const = 0;
	virtual bool isInstance() const = 0;

    };
    
    /** field evaluator, return the value of the field */
    class FieldEvaluator : public Evaluator
    {
     private:
        FrBField * _fl;
     public:
        FieldEvaluator(FrBField * f);
        FrBBaseObject* eval(FrBBaseObject * me, FrBExecutionEnvironment& e) const
            throw (FrBEvaluationException);
        const FrBClass* getClass() const;
        bool needMe() const;
        bool isAssignable() const;
	String name() const { return _fl->fullName(); }
        void refAssign(FrBExecutionEnvironment&, FrBBaseObject*, FrBBaseObject*) const throw (FrBEvaluationException);
	bool isInstance() const;
    };
    
    /** function/sub evaluator (ie it returns an objet of the 'Function' type,
      * it doesn't eval the function)
      */
    class FunctionEvaluator : public Evaluator
    {
     private:
        FrBFunction * _fn;
     public:
        FunctionEvaluator(FrBFunction * f);
        FrBBaseObject* eval(FrBBaseObject * me, FrBExecutionEnvironment& e) const
            throw (FrBEvaluationException);
        const FrBClass* getClass() const;
        bool needMe() const;
	String name() const { return _fn->fullName(); }
	bool isInstance() const;
    };
    
    /** class evaluator (ie it returns an objet of the 'Class' type)*/
    class ClassEvaluator : public Evaluator
    {
     private:
        FrBClass * _cl;
     public:
        ClassEvaluator(FrBClass * c);
        FrBBaseObject* eval(FrBBaseObject * me, FrBExecutionEnvironment& e) const
            throw (FrBEvaluationException);
        const FrBClass* getClass() const;
        bool needMe() const;
	String name() const { return _cl->fullName(); }
	bool isInstance() const;
    };
    
    FrBExpr *                _context;
    String                    _name;
    //FrBMember  *              _value;
    Evaluator *               _evaluator;

    bool                      _context_resolved;
    

    
public:
    FrBUnresolvedIdWithContextExpr(FrBExpr * context, const String& name);
    ~FrBUnresolvedIdWithContextExpr();
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;

    inline const String& name() const { return _name; }
    inline FrBExpr* context() { return _context; }
    inline void setContextResolved() { _context_resolved = true; }
    
    std::ostream& put(std::ostream& stream) const;
    
    bool isAssignable() const;
    void refAssign(FrBExecutionEnvironment&, FrBBaseObject*) const throw (FrBEvaluationException);

    bool isInstance() const;
};



/*  Member operator (.) class FrBMemberOpExpr : public FrBExpr
{
    FrBExpr               *_lhs;
    FrBUnresolvedIdExpr   *_rhs;
    bool                   _resolved;
    
public:
    FrBMemberOpExpr(FrBExpr* lhs, FrBUnresolvedIdExpr* rhs);
    ~FrBMemberOpExpr();
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    std::ostream& put(std::ostream& stream) const;
    
    inline bool resolved() { return _resolved; }
    inline FrBUnresolvedIdExpr* rhs() { return _rhs; }
    inline FrBExpr * lhs() { return _lhs; }
};*/

/** Function call operator ie expr(expr, expr, ...) */
class FrBFunctionCallExpr : public FrBExpr
{
    /** _lhs can be the function name or any expr of the Function-type */
    FrBExpr               *_lhs;
    
    /** arguments */
    FrBExprList           *_rhs;
    
    /** function to call (resolved from rhs & lhs) */
    FrBFunction           *_fn;
    
    /** expr that provides a me value for non-shared function */
    FrBExpr               *_me;

    
public:
    FrBFunctionCallExpr(FrBExpr* lhs, FrBExprList* rhs);
    ~FrBFunctionCallExpr();
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    std::ostream& put(std::ostream& stream) const;  
};


/** Local var (for localvar, parameters) */
class FrBLocalVarExpr : public FrBExpr
{
private:
    FrBCodeFunction *     _fn;
    FrBTypeExpr *     _type;
    int               _varid;
    
public:
    FrBLocalVarExpr(FrBCodeFunction * fn, FrBTypeExpr * t, int varid);
    ~FrBLocalVarExpr();
    
    bool isAssignable() const;
    void refAssign(FrBExecutionEnvironment&, FrBBaseObject*) const throw (FrBEvaluationException);
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    std::ostream& put(std::ostream& stream) const;
};
//TODONEXT cf log rev 83(1) +  deux ordre possibles ds la grammaire pr classes et fonctions
//+ finir les champs
//TODO pr les operateur de conversion automatique (ie INT <-> STRING) : utiliser des interfaces
//TODO utiliser un constructeur priv� pour les types Void et Null
//TODO GC, Makefile (pour common/*.cpp)
//Import / Import ss As sd
//operator # (cardinal)
//id�e syntax constructeur Public Sub Initialize() Parent1(a, b, c), Parent2(a)
//tout l'arbre devra g�rer les delete, gc seulement pr les type frb
//pour le prob de declare on verifie en amont en comparant une evntuelle egalite de ptr
//dim a as expr (on aura comme cas particulier : dim a as typeof expr)
//faire equivalent a ::String pour forcer root
// 2 notions possibles pour partial class :
// * classe partielle (definies a plusieurs endroits), exemple
// 
//    Partial Class A
//      Public Sub a()
//      End Sub
//    End Class
//
//    Partial Class A
//      Public Sub b()
//      End Sub
//    End Class
// now A has two subs : a & b
// * classe a definition partielle
//    Class A
//       Public Sub Initialize(a As Integer, s As String)
//       End Sub
//    End Class
//
//    TypeDef A(5) B
//
//    ca ne dispense pas des templates
//
// introspection :
//
// Dim c As Class, f As Function
// c := MyClass    <-- utilise un operator de conversion qui retournera un FrBClassWrapper(MyClass.getClass())
// d := myFunction
//
// For i As Int := 1 To 10 <=> ForEach i In new IntRange(1, 10)



/*
    empiler les addr des fonctions pour debug
    introspection profonde quand possible
    interpreteur embarqu�
    classe de d�marrage impl�mente Startable
    compilation des fichiers sources FrB en bibliotheque C++ si voulu
*/
//   /!\  attention il faut faire les verif shared sinon ca seg fault

/** Me expr for expression that are inside function */
class FrBInsideMeExpr : public FrBMeExpr
{
private:
    FrBCodeFunction *     _fn;
    int               _varid;
    
public:
    FrBInsideMeExpr(FrBCodeFunction * f, bool nonSharedContext);
    ~FrBInsideMeExpr();

    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);    
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    std::ostream& put(std::ostream& stream) const;    
};

/** Me expr for expression that are outside function */
class FrBOutsideMeExpr : public FrBMeExpr
{
private:
    FrBField     *    _field;
    
public:
    FrBOutsideMeExpr(FrBField * t, bool nonSharedContext);
    ~FrBOutsideMeExpr();

    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    std::ostream& put(std::ostream& stream) const;    
};

//TODO il y a du code a factoriser entre refassign et binop

/** Ref assignment <expr> := <expr> */
class FrBRefAssignExpr : public FrBExpr
{
private:
    FrBExpr     *_lhs;
    FrBExpr     *_rhs;
    
public:
    FrBRefAssignExpr(FrBExpr* lhs, FrBExpr* rhs) throw (FrBFunctionNotFoundException);
    ~FrBRefAssignExpr();
    
    bool isAssignable() const;
    void refAssign(FrBExecutionEnvironment&, FrBBaseObject*) const throw (FrBEvaluationException);
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
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
    FrBBinOpExpr(FrBExpr* lhs, FrBExpr* rhs, int op);
    ~FrBBinOpExpr();
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void partialResolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    std::ostream& put(std::ostream& stream) const; 

    inline void setLHS(FrBExpr * e) { _lhs = e; }   
    inline void setRHS(FrBExpr * e) { _rhs = e; }   
};

class FrBNewExpr : public FrBExpr
{
private:

    FrBTypeExpr *_type;
    FrBExprList * _args;
    FrBFunction * _ctor;
    
public:
    FrBNewExpr(FrBTypeExpr * type, FrBExprList * args);
    ~FrBNewExpr();

    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    const FrBClass* getClass() const;


    FrBBaseObject* eval() const throw (FrBEvaluationException);

    std::ostream& put(std::ostream& stream) const;    
};

template<class literal_type>
class FrBLiteralExpr : public FrBExpr
{
protected:
    FrBBaseObject* _value;


    FrBLiteralExpr(FrBBaseObject* o)
    {
        _value = o;
    }
    
public:
    FrBLiteralExpr(const literal_type& v)
    {
        _value = new FrBPrimitive<literal_type>(v);
    }
    
    ~FrBLiteralExpr()
    {
	if(!_value->isManaged())
	    delete _value;
    }
 
    
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException)
    {
	if(!_value->isManaged())
	    e.addGarbagedObject(_value);

        return _value;
    }
    
    const FrBClass* getClass() const
    {
        return  FrBPrimitive<literal_type>::getCppClass();
    }

    std::ostream& put(std::ostream& stream) const;    

};

typedef FrBLiteralExpr<FrBInt::ptype>       FrBIntExpr;
typedef FrBLiteralExpr<FrBBool::ptype>      FrBBaseBoolExpr;
// typedef FrBLiteralExpr<FrBDouble::ptype> FrBDoubleExpr;
// typedef FrBLiteralExpr<FrBSingle::ptype> FrBSingleExpr;
// typedef FrBLiteralExpr<FrBShort::ptype>  FrBShortIntExpr;
// typedef FrBLiteralExpr<FrBLong::ptype>   FrBLongIntExpr;
typedef FrBLiteralExpr<FrBString::ptype>    FrBStringExpr;
// typedef FrBLiteralExpr<FrBChar::ptype>   FrBCharExpr;

/** Bool expression */
class FrBBoolExpr : public FrBBaseBoolExpr
{
public:
    FrBBoolExpr(const bool& v)
	: FrBBaseBoolExpr(FrBBool::fromCPPBool(v)) { }
 

};

/** The Null expression */
class FrBNullExpr : public FrBExpr
{
private:
    //static FrBNullExpr * _null_expr;
    
    //FrBNullExpr();
    
public:
    
    FrBNullExpr();
    ~FrBNullExpr();
 
    //static FrBNullExpr * nullExpr();
    
    FrBBaseObject* eval(FrBExecutionEnvironment& e) const throw (FrBEvaluationException);
    
    const FrBClass* getClass() const;

    std::ostream& put(std::ostream& stream) const;    

};


#endif


