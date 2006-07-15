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

class FrBBinOpExpr : public FrBExpr
{
private:
    FrBExpr* _rhs;
    FrBExpr* _lhs;
    char*    _op;
    
public:
    FrBBinOpExpr(FrBExpr* rhs, FrBExpr* lhs, char* op);
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

template<class literal_type, class boxing_type>
class FrBLiteralExpr : public FrBExpr
{
private:
    FrBCppObject* _pvalue;
    
public:
    FrBLiteralExpr(const literal_type& v)
    {
        /*_pvalue = FrBBoxing::box(v);
        mem->push_back(_pvalue);*/
        _pvalue = new boxing_type(v);
        //TODO pvalue ne doit pas etre modifié, doit etre de type FrBConstInt
    }
    
    ~FrBLiteralExpr()
    {
        //FrBBoxing::dispose_obj(_pvalue);
        //on ne delete pas _pvalue
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
        return stream << "<val=" << 0 /*FrBCppObject::put() */ << ">";
    }    
};

typedef FrBLiteralExpr<int, FrBInt>        FrBIntExpr;
// typedef FrBLiteralExpr<double>          FrBDoubleExpr;
// typedef FrBLiteralExpr<float>           FrBSingleExpr;
// typedef FrBLiteralExpr<short int>       FrBShortIntExpr;
// typedef FrBLiteralExpr<long int>        FrBLongIntExpr;
// typedef FrBLiteralExpr<String>          FrBStringExpr;
// typedef FrBLiteralExpr<char>            FrBCharExpr;


#endif


