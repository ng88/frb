#include "frbbaseobject.h"

/*         FrBObject                  */

FrBObject::~FrBObject()
{
}

const FrBClass * FrBObject::getClass()
{
    return _class;
}


/*            FrBBindingObject            */

FrBBindingObject::~FrBBindingObject()
{
}
