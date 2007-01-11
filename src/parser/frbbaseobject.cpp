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
  FRB_ASSERT_CODE(_field_count = fc);
}


FrBBaseObject* FrBUserObject::getField(int i) throw (FrBEvaluationException)
{
  frb_warning2(i >= 0 && i < _field_count, "FrBUserObject::getField(), index out of bounds");
  return _fields[i];
}

void FrBUserObject::addField(int i, FrBBaseObject * o) throw (FrBEvaluationException)
{
  frb_assert2(i >= 0 && i < _field_count, "FrBUserObject::addField(), index out of bounds");
  _fields[i] = o;
}

FrBUserObject::~FrBUserObject()
{
  delete[] _fields;
}
