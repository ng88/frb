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

#ifndef FRBCONSTANTS_H
#define FRBCONSTANTS_H

typedef int scope_t;

/* les defauts doivent etre à 0 */
enum { SC_DEFAULT = 0 };
enum { SC_PRIVATE = 0, SC_PROTECTED = 12, SC_FRIEND = 13, SC_PUBLIC = 14};
enum { SC_ABSTRACT = 1, SC_SHARED = 2, SC_SEALED = 3};

#endif

