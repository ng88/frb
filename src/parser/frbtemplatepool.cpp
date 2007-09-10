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


#include "frbtemplatepool.h"

#include "../common/assert.h"

#include "frbclass.h"
#include "frbexpr.h"


FrBClass * FrBTemplatePool::FrBInstanciedTemplateEntry::getInstance()
{
    frb_assert(_template->templateParameterCount() == _args.size());

    FrBTemplateSpecializationEnvironment env;
    return _template->specializeTemplate(env);
}
