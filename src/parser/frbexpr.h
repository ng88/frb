#ifndef FRBEXPR_H
#define FRBEXPR_H

#include "frbbaseobject.h"
#include "frbbuiltinclasses.h"
#include "frbexceptions.h"

class FrBExpr
{
public:
    virtual ~FrBExpr() {}
    virtual FrBBaseObject* eval() const throw (FrBEvaluationException) = 0;
    virtual const FrBClass* getClass() const = 0;
    virtual std::ostream& put(std::ostream& stream) const = 0;
};

std::ostream& operator<<(std::ostream& s, const FrBExpr& expr);

typedef std::vector<FrBExpr*> FrBExprList;


/** This class handles an identifier and have to search if it's a function, a class, an object ... and return the corresponding wrapper or object */
class FrBIdExpr : public FrBExpr
{
private:
    FrBBaseObject* _wrapper;
    String _name;
    
public:
    FrBIdExpr(const String& str);
    ~FrBIdExpr();
    
    void resolve(const FrBClass * context)  throw (FrBUndeclaredIdentifierException);
    
    FrBBaseObject* eval() const throw (FrBEvaluationException);
    const FrBClass* getClass() const;
    std::ostream& put(std::ostream& stream) const;  
};

/** Member operator (.) */
class FrBMemberOpExpr : public FrBExpr
{
    FrBExpr     *_lhs;
    FrBIdExpr   *_rhs;
    
public:
    FrBMemberOpExpr(FrBExpr* lhs, FrBIdExpr* rhs);
    ~FrBMemberOpExpr();
    FrBBaseObject* eval() const throw (FrBEvaluationException);
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
    FrBBaseObject* eval() const throw (FrBEvaluationException);
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
    FrBCppObject* _pvalue;
    //literal_type  _value;
    
public:
    FrBLiteralExpr(const literal_type& v)
    {
        _pvalue = new FrBPrimitive<literal_type>(v);
        //_value = v;
        //TODO pvalue ne doit pas etre modifié, doit etre de type FrBConstInt
    }
    
    ~FrBLiteralExpr()
    {
        //FrBBoxing::dispose_obj(_pvalue);
        //on ne delete pas _pvalue
        //on devrait pourtant, enfin on vera ca plus tard
    }
    
    FrBBaseObject* eval() const throw (FrBEvaluationException)
    {
        return _pvalue;
    }
    
    const FrBClass* getClass() const
    {
        return _pvalue->getClass();
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


