
#ifndef FRBKEYWORDS_H
#define FRBKEYWORDS_H


class FrBKeywords
{
public:
    enum 
    {
        FRB_KW__NOTFOUND = -1,
        FRB_KW_DECLARE = 0,
        FRB_KW_DECLAREAS,
        FRB_KW_CONST,
        FRB_KW_STATIC,
        FRB_KW_PUBLIC,
        FRB_KW_PRIVATE,
        FRB_KW_PROTECTED,
        
        FRB_KW__COUNT
    };
    
    static char* key[FRB_KW__COUNT];
    static int findKeyword(char* str);
};

#endif
