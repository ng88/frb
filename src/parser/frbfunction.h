#ifndef FRBFUNCTION_H
#define FRBFUNCTION_H

#include "frbstat.h"
#include "frbexceptions.h"

typedef int scope_t;

/* les defauts doivent etre à 0 */
enum { SC_DEFAULT = 0 };
enum { SC_PRIVATE = 0, SC_PROTECTED, SC_FRIEND, SC_PUBLIC };
enum { SC_ABSTRACT = 1, SC_SHARED, SC_SEALED };

class FrBClass;
class FrBFunction;
typedef std::vector<FrBClass*> FrBClassList;
typedef std::vector<const FrBClass*> FrBConstClassList;
typedef std::map<const String, FrBClass*> FrBClassMap;
typedef std::vector<FrBFunction*> FrBFunctionList;

class FrBFunction /* a frb function */
{
protected:
    bool _sub;
    bool _shared;
    bool _const; 
    bool _abstract;
    //TODO, const=true => Me est const dans cette foction
    String _name;
    
    //FrBClassList _argsType;
    FrBClass * _returnType;
    
    /* the first parameter that is optional */
    int _firstOptional;
    
    /* the last parameter is a ParamArray */
    bool _paramArray;
    
    scope_t  _scope;
    
public:

    enum { SHARED = 0 };

    typedef enum { NO_MATCH = 0,
                   MATCH_WITH_CONV,
                   MATCH_WITH_PARAM_ARRAY,
                   MATCH_WITH_OPT,
                   MATCH } match_t;
 
    inline FrBFunction() : _sub(false), _shared(false), _const(false), _abstract(false), _firstOptional(-1),
                           _paramArray(false), _scope(SC_PUBLIC)  {};
                             
    virtual ~FrBFunction() {}
    virtual std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    inline void setSub(bool v) { _sub = v; }
    inline bool sub() const { return _sub; }
    
    inline void setShared(bool v) { _shared = v; }
    inline bool shared() const { return _shared; }
    
    inline void setConst(bool v) { _const = v; }
    inline bool isConst() const { return _const; }
    
    inline void setAbstract(bool v) { _abstract = v; }
    inline bool abstract() const { return _abstract; }
    
    inline scope_t scope() const { return _scope; }
    inline void setScope(scope_t v) { _scope = v; }
    
    inline void setParamArrayUsed(bool v) { _paramArray = v; }
    inline bool paramArrayUsed() const { return _paramArray; }
    
    inline void setFirstOptionalParameter(int v) { _firstOptional = v; }
    inline int firstOptionalParameter() const { return _firstOptional; }
    
    virtual const FrBClass * parameterType(int index) const = 0;
    virtual bool parameterByVal(int index) const = 0;
    virtual int parameterCount() const = 0;
    
    inline void setReturnType(FrBClass * t) { _returnType = t; }
    inline const FrBClass * returnType() const { return _returnType; }
    
    inline void setName(const String& n) { _name = n; }
    inline const String& name() const { return _name; }
    
    virtual FrBBaseObject * execute(FrBBaseObject * me) const throw (FrBExecutionException);
    virtual FrBBaseObject * execute(FrBBaseObject * me, FrBBaseObject * arg0) const
        throw (FrBExecutionException);
    virtual FrBBaseObject * execute(FrBBaseObject * me, FrBBaseObject * arg0, FrBBaseObject * arg1) const
        throw (FrBExecutionException);
    virtual FrBBaseObject * execute(FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException) = 0;
    
    /*virtual*/ match_t matchParameters(const FrBBaseObjectList& args);
    /*virtual*/ match_t matchParameters(const FrBConstClassList& args);
    

    template<class It, class ArgContainer>    
    static FrBFunction* findOverload(It begin, const It& end, const ArgContainer& args)
        throw (FrBFunctionNotFoundException)
    {
        FrBFunction * bestMatch = 0;
        match_t matchDegree = NO_MATCH;
        
        for(; begin != end && matchDegree != MATCH; ++begin)
        {
            match_t m = (*begin)->matchParameters(args);
    
            if( m >= matchDegree)
            {
                matchDegree = m;
                bestMatch = *begin;
            }
        }
        
        if(matchDegree == NO_MATCH)
            throw FrBFunctionNotFoundException("", args);
            
        return bestMatch;
    }
        
};

std::ostream& operator<<(std::ostream& s, const FrBFunction& fn);

//TODO il faut resoudre les types directement
//TODO rendre l'interpreteur thread safe (gestion de la mémoire, du GC, des local var ds les codefunctions)

class FrBCodeFunction : public FrBFunction
{
public:

    struct Param
    {
        FrBClass * type;
        bool byval;
        FrBExpr * init;
        
        Param()
        : type(0), byval(false), init(0) {}
        
        Param(FrBClass * t, bool b, FrBExpr * i)
         : type(t), byval(b), init(i) {}
        
    };
    
    typedef std::map<const String, int> NameParamList;
    typedef std::vector<Param>          ParamList;
    
    typedef int VarID;
    
    typedef std::map<const String, VarID> NameVarList;
    typedef std::vector<FrBBaseObject*> VarList;
    
protected:
    
    FrBStatementlist     _stats;
    NameParamList        _paramName;
    ParamList            _param;
    
    NameVarList          _varName;
    VarList              _var;
    

    
public:
    FrBCodeFunction();
    ~FrBCodeFunction();
    
    inline void addStat(FrBStatement* v) { _stats.push_back(v); }
    //TODO verif nom des params (doublons)
    inline void addParam(const String& name, FrBClass* v, bool byval, FrBExpr * init = 0)
    {
        _paramName[name] = _param.size();
        _param.push_back(Param(v, byval, init));
    }
    
    /** Return a pointer to a Param struct if found or 0 if not found */
    inline const Param * getParam(const String& name) const
    {
        NameParamList::const_iterator it = _paramName.find(name);
        
        return (it == _paramName.end()) ? 0 : &_param[it->second];
    }
    
    /** Display on stream a string representation of this */
    std::ostream& put(std::ostream& stream, int indent = 0) const;
    
    const FrBClass * parameterType(int index) const;
    int parameterCount() const;
    bool parameterByVal(int index) const;
    
    FrBBaseObject * execute(FrBBaseObject * me, const FrBBaseObjectList& args) const
        throw (FrBExecutionException);
        
    /* local var handling functions */
    
    /** add a local var */
    inline void addLocalVar(String name, FrBBaseObject * type)
    {
        _varName[name] = _var.size();
        _var.push_back(type);
    }
    
    /** Return a pointer to a Param struct if found or 0 if not found */
    inline VarID findLocalVar(String name) const
    {
        NameVarList::const_iterator it = _varName.find(name);
        
        return (it == _varName.end()) ? -1 : it->second;
    }
    
    inline FrBBaseObject * getLocalVar(VarID id) const { return _var[id]; }
    
};



class FrBProperty
{
private:
    FrBFunction * _get;
    FrBFunction * _set;
};

typedef std::vector<FrBCodeFunction*> FrBCodeFnList;
typedef std::stack<FrBCodeFunction*> FrBCodeFunctionStack;

#endif


