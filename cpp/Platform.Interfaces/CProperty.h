#pragma once

#include "CSetter.h"
#include "CProvider.h"

namespace Platform::Interfaces
{
    template<typename TSelf, typename TObject, typename TValue>
    concept CProperty = CSetter<TSelf, TValue, TObject> && CProvider<TSelf, TValue, TObject>;
}
