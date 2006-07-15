#include "frbexceptions.h"
#include "frbkeywords.h"
#include "frberrors.h"
#include "frbbaseobject.h"

std::ostream& operator<<(std::ostream& s, const FrBException& c)
{
    return c.put(s);
}

/*             FrBFunctionNotFoundException                */


FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name)
    : _name(name)
{
}

FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name, const std::vector<const FrBClass*>& args)
    : _name(name), _args(args)
{
}

FrBFunctionNotFoundException::FrBFunctionNotFoundException(const String& name, const std::vector<FrBBaseObject*>& args)
    : _name(name)
{
    for(std::vector<FrBBaseObject*>::const_iterator it = args.begin(); it != args.end(); ++it)
        _args.push_back( (*it)->getClass() );
}



std::ostream& FrBFunctionNotFoundException::put(std::ostream& stream) const
{
    return stream << _name << ": " << FrBErrors::getMsg(FrBErrors::FRB_ERR_FN_NOT_FOUND);
}


/*                   FrBDefaultCtorNotFoundException                */


FrBDefaultCtorNotFoundException::FrBDefaultCtorNotFoundException()
 : FrBFunctionNotFoundException(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_CONSTRUCTOR_NAME))
{
}

/*                   FrBDtorNotFoundException                */


FrBDtorNotFoundException::FrBDtorNotFoundException()
 : FrBFunctionNotFoundException(FrBKeywords::getKeyword(FrBKeywords::FRB_KW_DESTRUCTOR_NAME))
{

}


/*             FrBClassNotFoundException             */

FrBClassNotFoundException::FrBClassNotFoundException(const String& name)
    : _name(name)
{
}

std::ostream& FrBClassNotFoundException::put(std::ostream& stream) const
{
    return stream << _name << ": " << FrBErrors::getMsg(FrBErrors::FRB_ERR_CLASS_NOT_FOUND);
}


/*               FrBIncompatibleClassException                    */


FrBIncompatibleClassException::FrBIncompatibleClassException(const FrBClass * provided, const FrBClass * needed)
    : _provided(provided), _needed(needed)
{
}

std::ostream& FrBIncompatibleClassException::put(std::ostream& stream) const
{
    return stream;
}

