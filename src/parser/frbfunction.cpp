#include "frbfunction.h"
#include "frbclass.h"
#include "../common/assert.h"

std::ostream& operator<<(std::ostream& s, const FrBFunction& fn)
{
    return fn.put(s);
}

/*          FrBFunction             */


FrBBaseObject * FrBFunction::execute(FrBBaseObject * me) const throw (FrBExecutionException)
{
    FrBBaseObjectList args;
    return execute(me, args);
}

FrBBaseObject * FrBFunction::execute(FrBBaseObject * me, FrBBaseObject * arg0) const
        throw (FrBExecutionException)
{
    FrBBaseObjectList args;
    args.push_back(arg0);
    return execute(me, args);
}

FrBBaseObject * FrBFunction::execute(FrBBaseObject * me, FrBBaseObject * arg0, FrBBaseObject * arg1) const
        throw (FrBExecutionException)
{
    FrBBaseObjectList args;
    args.push_back(arg0);
    args.push_back(arg1);
    return execute(me, args);
}

// Pas tres propre mais j'ai pas mieux pour le moment :
#define COMMON_MATCH_PARAM_BODY(ARGS) \
   int count = parameterCount(); \
    int pcount = args.size(); \
     \
    bool full_match = true; \
     \
    const FrBClass * a; \
    const FrBClass * b; \
     \
    if(_paramArray) \
    { \
        if(pcount < count - 1) \
            return NO_MATCH; \
             \
        for(int i = 0; i < count - 1; ++i) /* fn pour ca */ \
        { \
            if( (a = parameterType(i)) != (b = ARGS) ) \
            { \
                if(FrBClass::areCompatibles(a, b)) \
                    full_match = false; \
                else \
                    return NO_MATCH; \
            } \
        } \
         \
        for(int i = count - 1; i < pcount; ++i)/* fn pour ca */ \
        { \
            if( (a = parameterType(count - 1)) != (b = ARGS) ) \
            { \
                if(FrBClass::areCompatibles(a, b)) \
                    full_match = false; \
                else \
                    return NO_MATCH; \
            } \
        } \
        \
        return full_match ? MATCH_WITH_PARAM_ARRAY : MATCH_WITH_CONV;  \
    } \
    else \
    { \
        if(pcount > count) \
            return NO_MATCH; \
             \
        bool opt_match = (_firstOptional > -1); \
         \
        if(!opt_match && pcount != count ) \
            return NO_MATCH; \
             \
        if(opt_match && pcount < _firstOptional ) \
            return NO_MATCH; \
          \
        for(int i = 0; i < pcount; ++i)/* fn pour ca */ \
        { \
            if( (a = parameterType(i)) != (b = ARGS) ) \
            { \
                if(FrBClass::areCompatibles(a, b)) \
                    full_match = false; \
                else \
                    return NO_MATCH; \
            } \
        } \
         \
        return full_match ? (opt_match ? MATCH_WITH_OPT : MATCH) : MATCH_WITH_CONV; \
    }
   

FrBFunction::match_t FrBFunction::matchParameters(const FrBConstClassList& args)
{
    COMMON_MATCH_PARAM_BODY((args[i]));
}


FrBFunction::match_t FrBFunction::matchParameters(const FrBBaseObjectList& args)
{
    COMMON_MATCH_PARAM_BODY((args[i]->getClass()));
}


std::ostream& FrBFunction::put(std::ostream& stream, int indent) const
{
    using namespace std;
    
    String str_indent(indent, '\t');
    

    stream  << str_indent << "\t    Function " << name() << endl
            << str_indent << "\t\t- Scope=" << scope() << endl
            << str_indent << "\t\t- Shared=" << shared() << endl
            << str_indent << "\t\t- Sub=" << sub() << endl
            << str_indent << "\t\t- Const=" << isConst() << endl
            << str_indent << "\t\t- Abstract=" << abstract() << endl
            << str_indent << "\t\t- Parameters:" << endl;
            
    for(int i = 0; i < parameterCount(); ++i)
    {
        stream << str_indent << "\t\t\t* " << (parameterByVal(i) ? "byval " : "byref ")
                                           << parameterType(i)->name();
        
        if(firstOptionalParameter() > -1 && i >= firstOptionalParameter())
            stream << " (optional)";
            
        if(paramArrayUsed() && i == parameterCount() - 1)
            stream << " (param_array)";
            
        stream << endl;
    }
            
    if(!sub())        
        stream  << str_indent << "\t\t- Return type=" << returnType()->name() << endl;
            
    return stream;
}



/*           FrBCodeFunction            */
FrBCodeFunction::FrBCodeFunction()
{
    _sub = false;
    _shared = false;
    _const = false;
}


FrBCodeFunction::~FrBCodeFunction()
{
    for(FrBStatementlist::iterator it = _stats.begin(); it != _stats.end(); ++it)
        delete(*it);
}

const FrBClass * FrBCodeFunction::parameterType(int index) const
{
    frb_assert2(index < parameterCount(), "FrBCodeFunction::parameterType(int) / index out of bounds");
    return _param[index].type;
}

int FrBCodeFunction::parameterCount() const
{
    return _param.size();
}

bool FrBCodeFunction::parameterByVal(int index) const
{
    frb_assert2(index < parameterCount(), "FrBCodeFunction::parameterByVal(int) / index out of bounds");
    return _param[index].byval;
}


FrBBaseObject * FrBCodeFunction::execute(FrBBaseObject * me, const FrBBaseObjectList& args) const
    throw (FrBExecutionException)
{
    //TODO fournit ici la liste des classes, valables aussi pr les autres sous classes
    //TODO dico des variables locales (dont paramètre et Me)
    
    FrBBaseObject * ret;
    
    for(FrBStatementlist::const_iterator it = _stats.begin(); it != _stats.end(); ++it)
        (*it)->execute();
    
    if(_sub)
        return 0;
    else
        return ret;
}

std::ostream& FrBCodeFunction::put(std::ostream& stream, int indent) const
{
    using namespace std;
    
    String str_indent(indent, '\t');

    FrBFunction::put(stream, indent);
    
    stream  << str_indent << "\t\t- Local vars:" << endl;
    
    for(int i = 0; i < localVarCount(); ++i)
    {
        stream << str_indent << "\t\t\t* " << i << " (" << getLocalVar(i)->name() << ")" << endl;
    }
            
    stream  << str_indent << "\t\t- Statements:" << endl;
    
    for(FrBStatementlist::const_iterator it = _stats.begin(); it != _stats.end(); ++it)
        stream << str_indent << "\t\t\t*stat> " << **it << endl;
    
    return stream;
}



