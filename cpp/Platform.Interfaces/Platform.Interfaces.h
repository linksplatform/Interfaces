#pragma once

#ifndef Platform_Interfaces
#define Platform_Interfaces

//#include <cstdint>

namespace Platform::Interfaces
{
    template <typename...> class ICounter;
}
#include "ICounter[TResult, TArgument].cpp"
#include "ICounter[TResult].cpp"
#include "ICriterionMatcher.cpp"
#include "IFactory.cpp"
//#include "IProperties.cpp"
//#include "IProperty.cpp"
namespace Platform::Interfaces
{
    template <typename...> class IProvider;
}
#include "IProvider[TProvided, TArgument].cpp"
#include "IProvider[TProvided].cpp"
namespace Platform::Interfaces
{
    template <typename...> class ISetter;
}
#include "ISetter[TValue, TArgument].cpp"
#include "ISetter[TValue].cpp"

#endif // Platform_Collections_Methods