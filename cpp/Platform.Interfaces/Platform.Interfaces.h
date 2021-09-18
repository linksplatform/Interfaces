#pragma once

#ifndef PLATFORM_INTERFACES
#define PLATFORM_INTERFACES

#include <concepts>
#include <ranges>

#include "IEnumerable.h"
#include "IArray.h"
#include "IList.h"
#include "ISet.h"
#include "IDictionary.h"

#include "ICounter.h"
#include "ICriterionMatcher.h"
#include "IFactory.h"
#include "IProperties.h"
#include "IProvider.h"
#include "ISetter.h"

#include "IProperty.h" // depend on ISetter and IProvider

#include "Polymorph.h"

#endif // PLATFORM_INTERFACES
