/**
state

http://www.github.com/rlisle/Patriot

Written by Ron Lisle

BSD license, check LICENSE for more information.
All text above must be included in any redistribution.
*/

#include "state.h"

State::State(String name, int value)
{
    _next  = NULL;
    _value = value;
    _name  = name;
}
