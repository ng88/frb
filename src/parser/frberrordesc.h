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


#ifndef FRBERRORDESC_H
#define FRBERRORDESC_H

#include "../common/string.h"
#include "frberrors.h"
#include <iostream>

class FrBErrorDescriptor
{
private:
    int _errno1, _errno2, _line;
    String _file, _class, _function, _token;
    bool _is_critical;
    
public:
    FrBErrorDescriptor()
     : _errno1(0), _errno2(0), _line(0), _file(""),
       _class(""), _function(""), _token(""), _is_critical(false) {};

    FrBErrorDescriptor(const FrBErrorDescriptor& v)
     : _errno1(v._errno1), _errno2(v._errno2), _line(v._line),
       _file(v._file), _class(v._class), _function(v._function),
       _token(v._token), _is_critical(v._is_critical) {};
       
    FrBErrorDescriptor(int no1, int no2, int line, const String& file, const String& classname,
                         const String& function, const String& token, bool critical = false)
     : _errno1(no1), _errno2(no2), _line(line), _file(file), _class(classname),
       _function(function), _token(token), _is_critical(critical) {};
       
       
    inline const char* errMsg() const { return FrBErrors::getMsg(_errno1); }
    inline const char* contextErrMsg() const { return FrBErrors::getMsg(_errno2); }
    inline int errNo() const { return _errno1 + FrBErrors::FRB_ERR_DISPLAYSHIFT; }  /* error id */
    inline int contextErrNo() const { return _errno2 + FrBErrors::FRB_ERR_DISPLAYSHIFT; } /* context error id */
    inline int fullErrNo() const { return errNo() * 100 + contextErrNo(); }  /* error id & context error id */
    
    inline const String& fileName() const { return _file; }
    inline const String& className() const { return _class; }
    inline const String& functionName() const { return _function; }
    inline const String& token() const { return _token; }
    inline int line() const { return _line; }
    inline bool critical() const { return _is_critical; }
    
    inline void setErrNo(int v) { _errno1 = v; }
    inline void setContextErrNo(int v) { _errno2 = v; }
    
    inline void setFileName(const String& v) { _file = v; }
    inline void setClassName(const String& v) { _class = v; }
    inline void setFunctionName(const String& v) { _function = v; }
    inline void setToken(const String& v) { _token = v; }
    inline void setLine(int v) { _line = v; }
    inline void setCritical(bool v) { _is_critical = v; }
   
    static FrBErrorDescriptor fromYYError(const char * msg, const char * yytext);
    //TODO //static FrBErrorDescriptor fromExecutionException(const ExecutionException&);

};

std::ostream& operator<<(std::ostream& s, const FrBErrorDescriptor& c);

//TODO utiliser des FrBFunction/Class.. à la place des String
// a refaire complétement ou presque...

/*class FrWarningDescriptor : public FrBErrorDescriptor
{
    FrWarningDescriptor()
     : FrBErrorDescriptor() {};

    FrWarningDescriptor(const FrWarningDescriptor& v)
     : FrBErrorDescriptor(v) {};
       
    FrWarningDescriptor(int no1, int no2, int line, const String& file, 
                        const String& classname, const String& function, const String& token)
     : FrBErrorDescriptor(no1, no2, line, file, classname, function, token) {};
};*/

class FrBErrorCollector
{
public:
    
    void error(int no1, int no2, int line, const String& file,
                    const String& classname, const String& function, const String& token);
    void warning(int no1, int no2, int line, const String& file,
                    const String& classname, const String& function, const String& token);
    void error(const FrBErrorDescriptor& error);

    
};


#endif


