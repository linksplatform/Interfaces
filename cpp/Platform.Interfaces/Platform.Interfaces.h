#pragma once

#include <concepts>
#include <ranges>

#include "Macros.h"

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

#include "Extended.h"
#include "ExtendedBase.h"
#include "ExtendedContainer.h"
#include "ExtendedContainerBase.h"
#include "ExtendedReference.h"
#include "ExtendedReferenceBase.h"
