

#include "frbkeywords.h"
#include <cstring>
#include <cstdio>

char* FrBKeywords::key[FRB_KW__COUNT] = { "Dim", "As" };


int FrBKeywords::findKeyword(char* str)
{
    for(int i = 0; i <  FRB_KW__COUNT; ++i)
        if( !strcmp(key[i], str) )
            return i;
            
    return FRB_KW__NOTFOUND;
}



