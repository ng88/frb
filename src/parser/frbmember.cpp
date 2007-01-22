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

#include "frbmember.h"
#include "frbkeywords.h"
#include "frbclass.h"

   
FrBMember::FrBMember()
    : _contClass(0), _shared(false), _const(false), _scope(SC_PUBLIC)
{

}


String FrBMember::fullName() const
{
    if(container())
        return container()->fullName()
                  .append(FrBKeywords::getKeywordOrSymbol(FrBKeywords::FRB_KW_OP_MEMBER))
                  .append(name());
    else
        return name();
}


