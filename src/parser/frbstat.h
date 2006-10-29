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


#include <iostream>
#include "../common/string.h"

#include "frbbaseobject.h"
#include "frbexecutionenvironment.h"
#include "frbexceptions.h"

class FrBExpr;
class FrBTypeExpr;
class FrBClass;
class FrBCodeFunction;

//TODO optimisation de l'arbre avt serialisation
// resolution des appeles de fonctions, pr�calculs d'expressions, r�solution de classe
// entete d'un fichier s�rialis� : FRBS<DWORD:version qui a g�n�r� le fichier><DWORD:version minimum pour charger ce fichier>

class FrBStatement
{
public:
    virtual ~FrBStatement() {}
    virtual void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException) {}
    virtual void execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException) = 0;
    virtual std::ostream& put(std::ostream& stream) const = 0;
};

typedef std::vector<FrBStatement*> FrBStatList;

std::ostream& operator<<(std::ostream& s, const FrBStatement& stat);

/** Simple conditional statement, ie if <expr> then <stats> */
class FrBConditionalStatement : public FrBStatement
//TODO implements Block, like FrBCodeFunction. Block defines addStat()
{
private:
    FrBStatList        _stats;
    FrBExpr *          _cond;
    
public:
    FrBConditionalStatement(FrBExpr * cond);
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException);
    std::ostream& put(std::ostream& stream) const;
    
    ~FrBConditionalStatement();
};

typedef std::vector<FrBConditionalStatement*> FrBCondList;

/** Full conditional statement, ie with if, else if, else... */
class FrBIfStatement : public FrBStatement
{
private:
    FrBCondList _conds;
    
public:
    FrBIfStatement();
    
    void resolveAndCheck(FrBResolveEnvironment&) throw (FrBResolveException);
    void execute(FrBExecutionEnvironment& e) const throw (FrBExecutionException);
    std::ostream& put(std::ostream& stream) const;
    
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
    std::ostream& put(std::ostream& stream) const;
    
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
    std::ostream& put(std::ostream& stream) const;
    
    ~FrBExprStatement();
};

typedef std::vector<FrBStatement*> FrBStatementlist;



#endif


