#pragma once

#ifndef Platform_Interfaces
#define Platform_Interfaces

#include "ICounter[TResult, TArgument].cpp"
#include "ICounter[TResult].cpp"
#include "ICriterionMatcher.cpp"
#include "IFactory.cpp"
#include "IProperties.cpp"
#include "IProvider[TProvided, TArgument].cpp"
#include "IProvider[TProvided].cpp"
#include "ISetter[TValue, TArgument].cpp"
#include "ISetter[TValue].cpp"

#include "IProperty.cpp" // depend on ISetter and IProvider

#endif // Platform_Collections_Methods