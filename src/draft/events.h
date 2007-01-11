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



#ifndef EVENT_H
#define EVENT_H

// déclare un événement 
#define DEC_EVENT(name) name##Callback __eventsys_##name##Event

// connecte un événement
#define CONNECT_EVENT(name, fn) __eventsys_##name##Event = (fn)

// appelle un événement                 
#define EMIT_EVENT(name, param) \
    { if( __eventsys_##name##Event != NULL ) \
          __eventsys_##name##Event(param); }

// déclare les paramètres de la fonction cliente
#define PARAM_FN_EVENT(name) const name##Infos&

struct ErrorInfos
{
    int line;
    int col;
    int errid;
    String errstr;
    String code;
};

typedef void (*ErrorCallback)(const ErrorInfos& e);

struct DebugInfos
{
    String caller;
    String debugstr;
};

typedef void (*DebugCallback)(const DebugInfos& e);

#endif
