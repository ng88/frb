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


#ifndef FRBSTAT_H
#define FRBSTAT_H

//TODO tous les destructeurs a refaire correctement ds stat & expr

#include <iostream>
#include "../common/string.h"

#include "frbbaseobject.h"
#include "frbexecutionenvironment.h"
#include "frbexceptions.h"

class FrBExpr;
class FrBTypeExpr;
class FrBClass;
class FrBCodeFunction;
class FrBStatement;

typedef std::vector<FrBStatement*> FrBStatementlist;

/** interface that represents a block of statements (function, if, ...) */
class FrBStatementBlock
{
 protected:
  FrBStatementlist _stats;

 public:
   /** add a statement in the block */
   inline void addStat(FrBStatement * s) { _stats.push_back(s); }
};


typedef std::stack<FrBStatementBlock*> FrBBlockStack;

//TODO optimisation de l'arbre avt serialisation
// resolution des appeles de fonctions, précalculs d'expressions, résolution de classe
// entete d'un fichier sérialisé : FRBS<DWORD:version qui a généré le fichier><DWORD:version minimum pour charger ce fichier>

class FrBStatement
{
public:
    virtual ~FrBStatement() {}
    virtual void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException) {}
    virtual void execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException) = 0;
    virtual std::ostream& put(std::ostream& stream, int indent = 0) const = 0;
};

std::ostream& operator<<(std::ostream& s, const FrBStatement& stat);

/** Block of statement (inconditional) */
class FrBBlockStatement : public FrBStatement, public FrBStatementBlock
{
public:

    FrBBlockStatement();
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException);
    
    std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    ~FrBBlockStatement();
};

/** abstract conditional block */
class FrBConditionalBlockStatement : public FrBBlockStatement
{
protected:
    /** return the value  of the evaluated condition */
    virtual bool evalCond(FrBExecutionEnvironment& e) const throw (FrBExecutionException) = 0;

public:
    bool executeCond(FrBExecutionEnvironment& e) const throw (FrBExecutionException);
    void execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException);
    

    
};

/** Simple conditional statement, ie [else] if <expr> then <stats> */
class FrBElseIfStatement : public FrBConditionalBlockStatement
{
private:
    FrBExpr *          _cond;

protected:
    bool evalCond(FrBExecutionEnvironment& e) const throw (FrBExecutionException);

public:
    FrBElseIfStatement(FrBExpr * cond);
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    ~FrBElseIfStatement();
};

/** Else */
class FrBElseStatement : public FrBConditionalBlockStatement
{
protected:
    bool evalCond(FrBExecutionEnvironment& e) const throw (FrBExecutionException);

};


typedef std::vector<FrBConditionalBlockStatement*> FrBCondList;

/** Full if statement, ie with if, else if, else... */
class FrBIfStatement : public FrBStatement
{
private:
    FrBCondList _conds;
    
public:
    FrBIfStatement();
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException);
    std::ostream& put(std::ostream& stream, int indent = 0) const;

    inline void addCond(FrBConditionalBlockStatement * c) { _conds.push_back(c); }
    
    ~FrBIfStatement();
};


/*
    delete stat
            rule : delete local_var
            exe : FrBClass::destroyInstance()
*/

class FrBDeclareStatement : public FrBStatement
{
private:
    int                _varid;
    FrBTypeExpr *      _type;
    FrBExpr *          _init;
    
public:
    FrBDeclareStatement(int varid, FrBTypeExpr * t, FrBExpr * init_val = 0);
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException);
    std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    ~FrBDeclareStatement();
};


class FrBExprStatement : public FrBStatement
{
private:
    FrBExpr*         _expr;
    
    
public:

    FrBExprStatement(FrBExpr* expr);
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException);
    std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    ~FrBExprStatement();
};


class FrBReturnStatement : public FrBStatement
{
private:
    FrBExpr*             _val;
    const FrBFunction *  _fn;
    
public:

    FrBReturnStatement(const FrBFunction * f, FrBExpr* v = 0);
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException);
    std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    ~FrBReturnStatement();
};


#endif


