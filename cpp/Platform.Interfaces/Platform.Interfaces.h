#pragma once

#include <concepts>
#include <ranges>

#include "Macros.h"

#include "CEnumerable.h"
#include "CArray.h"
#include "CList.h"
#include "CSet.h"
#include "CDictionary.h"

#include "CCounter.h"
#include "CCriterionMatcher.h"
#include "CFactory.h"
#include "CProperties.h"
#include "CProvider.h"
#include "CSetter.h"

#include "CProperty.h" // depends on CSetter and CProvider

#include "ICounter[TResult, TArgument].h"
#include "ICounter[TResult].h"
#include "ICriterionMatcher.h"
#include "IFactory.h"
#include "IProperties.h"
#include "IProperty.h"
#include "IProvider[TProvided, TArgument].h"
#include "IProvider[TProvided].h"
#include "ISetter[TValue, TArgument].h"
#include "ISetter[TValue].h"

#include "Polymorph.h"

#include "Extended.h"
#include "ExtendedBase.h"
#include "ExtendedContainer.h"
#include "ExtendedContainerBase.h"
#include "ExtendedReference.h"
#include "ExtendedReferenceBase.h"
