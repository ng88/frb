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


#ifndef FRBTEMPLATESPECIALIZATIONENVIRONMENT_H
#define FRBTEMPLATESPECIALIZATIONENVIRONMENT_H

#include "frbfunction.h"
class FrBClass;
class FrBCodeClass;


class FrBTemplateSpecializationEnvironment
{

private:

    FrBConstClassVector _args;
    FrBCodeClass *      _currentClass;
    FrBMember *         _currentMember;

    
public:

    inline void setCurrentClass(FrBCodeClass * v) { _currentClass = v; }
    inline FrBCodeClass * currentClass() const { return _currentClass; }

    inline void setCurrentMember(FrBMember * v) { _currentMember = v; }
    inline FrBMember * currentMember() const { return _currentMember; }


    inline const FrBClass * getArgument(unsigned int index) const { return _args[index]; }

};


#endif
