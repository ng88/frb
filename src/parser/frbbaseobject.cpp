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


#include "frbbaseobject.h"


/*            FrBBindingObject            */

FrBBindingObject::~FrBBindingObject()
{
}

/*         FrBUserObject            */

FrBUserObject::FrBUserObject(const FrBClass * type, int fc)
  :_type(type)
{
  _fields = new FrBBaseObject*[fc];
}


FrBBaseObject* FrBUserObject::getField(int i)
{
  return _fields[i];
}

void FrBUserObject::addField(int i, FrBBaseObject * o)
{
  _fields[i] = o;
}

FrBUserObject::~FrBUserObject()
{
  delete[] _fields;
}
