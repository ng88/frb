

#include "frberrordesc.h"
#include "frbkeywords.h"
#include <sstream>

/*           FrBErrorDescriptor               */

FrBErrorDescriptor FrBErrorDescriptor::fromYYError(const char * msg, const char * yytext)
{
    FrBErrorDescriptor err;

    err._errno2 = (strcmp(msg, "parse error") == 0) ? FrBErrors::FRB_ERR_PARSE : FrBErrors::FRB_ERR_UNKNOW;
       
    switch( FrBKeywords::findSymbol(yytext) )
    {
    case FrBKeywords::FRB_KW_OP_ARRAY_INI_END:
        err._errno1 = FrBErrors::FRB_ERR_ARRAY_INIT;
        break;
        
    case FrBKeywords::FRB_KW_OP_C_BRACKET:
        err._errno1 = FrBErrors::FRB_ERR_PARENTS_EXPR;
        break;
        
    case FrBKeywords::FRB_KW_OP_ARRAY_SUB_END:
        err._errno1 = FrBErrors::FRB_ERR_BRACES_EXPR;
        break;
        
    case FrBKeywords::FRB_KW__NOTFOUND: 
        err._errno1 = FrBErrors::FRB_ERR_TOKEN;
        break;
        
    default:
        err._errno1 = FrBErrors::FRB_ERR_ARITHM;
    }
    
    err._token = String(yytext);
    
    return err;
}
    
std::ostream& operator<<(std::ostream& s, const FrBErrorDescriptor& c)
{
    s << FrBErrors::getMsg(FrBErrors::FRB_ERR_LINE) << " " << c.line() << ": "; /* line X: */
    
    if(  c.token()[0] == '\n' )
        s << FrBErrors::getMsg(FrBErrors::FRB_ERR_MISS_BEFORE_ENDL) << ": "; /* before EOL: */
    else
        s << FrBErrors::getMsg(FrBErrors::FRB_ERR_NEAR) << " `" << c.token() << "': "; /* near X: */
    
    s << c.contextErrMsg() << ": " << c.errMsg()
      << " (" << FrBErrors::getMsg(FrBErrors::FRB_ERR_ERROR)  /* (error X-X) */
      << " " << c.fullErrNo() << ")\n";
    
    return s;
}




/*                 FrBErrorCollector             */


void FrBErrorCollector::error(int no1, int no2, int line, const String& file,
                const String& classname, const String& function, const String& token)
{
    error(FrBErrorDescriptor(no1, no2, line, file, classname, function, token, true));
}

void FrBErrorCollector::warning(int no1, int no2, int line, const String& file,
                const String& classname, const String& function, const String& token)
{
    error(FrBErrorDescriptor(no1, no2, line, file, classname, function, token, false));
}

void FrBErrorCollector::error(const FrBErrorDescriptor& error)
{
    //FrBErrorDescriptor err(error);
    //fprintf(stderr, "%s", err.toString().c_str());
    std::cerr << error;
}



