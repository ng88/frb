#ifndef FRBSTAT_H
#define FRBSTAT_H


#include <iostream>
#include "../common/string.h"

#include "frbbaseobject.h"

#include "frbexceptions.h"

class FrBExpr;
class FrBClass;

//TODO optimisation de larbre avt serialisation
// resolution des appeles de fonctions, précalculs d'expressions, résolution de classe
// entete d'un fichier sérialisé : FRBS<DWORD:version qui a généré le fichier><DWORD:version minimum pour charger ce fichier>


class FrBStatement
{
public:
    virtual ~FrBStatement() {}
    virtual void execute() const throw (FrBExecutionException) = 0;
    virtual std::ostream& put(std::ostream& stream) const = 0;
};

std::ostream& operator<<(std::ostream& s, const FrBStatement& stat);



class FrBDeclareStatement : public FrBStatement
{
private:
    String              _name;
    FrBClass *          _type;
    FrBExpr *           _init;
public:
    FrBDeclareStatement(const String& var_name, FrBClass * var_type, FrBExpr * init_val = 0);
    void execute() const throw (FrBExecutionException);
    std::ostream& put(std::ostream& stream) const;
    
    ~FrBDeclareStatement();
};


class FrBExprStatement : public FrBStatement
{
private:
    FrBExpr* _expr;
    
public:
    FrBExprStatement(FrBExpr* expr);
    void execute() const throw (FrBExecutionException);
    std::ostream& put(std::ostream& stream) const;
    
    ~FrBExprStatement();
};

typedef std::vector<FrBStatement*> FrBStatementlist;



#endif


