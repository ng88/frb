/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006 - 2010 by GUILLAUME Nicolas                        *
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

    inline FrBTemplateSpecializationEnvironment();

    inline void setCurrentClass(FrBCodeClass * v);
    inline FrBCodeClass * currentClass() const;

    inline void setCurrentMember(FrBMember * v);
    inline FrBMember * currentMember() const;


    inline void addArgument(const FrBClass * c);
    inline const FrBClass * getArgument(unsigned int index) const;

};











/*           inlined              */

inline FrBTemplateSpecializationEnvironment::FrBTemplateSpecializationEnvironment()
{
    FRB_ASSERT_CODE(_currentClass = 0);
    FRB_ASSERT_CODE(_currentMember = 0);
}

inline void FrBTemplateSpecializationEnvironment::setCurrentClass(FrBCodeClass * v)
{
    frb_assert(v);
    _currentClass = v;
}

inline FrBCodeClass * FrBTemplateSpecializationEnvironment::currentClass() const
{
    return _currentClass;
}

inline void FrBTemplateSpecializationEnvironment::setCurrentMember(FrBMember * v)
{
    frb_assert(v);
    _currentMember = v;
}

inline FrBMember * FrBTemplateSpecializationEnvironment::currentMember() const
{
    return _currentMember;
}


inline void FrBTemplateSpecializationEnvironment::addArgument(const FrBClass * c)
{
    frb_assert(c);
    _args.push_back(c);
}

inline const FrBClass * FrBTemplateSpecializationEnvironment::getArgument(unsigned int index) const
{
    frb_assert(index < _args.size());
    return _args[index];
}


#endif
