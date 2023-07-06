#pragma once

#include "CProvider.h"
#include "CSetter.h"

namespace Platform::Interfaces {
  template <typename TSelf, typename TObject, typename TValue>
  concept CProperty = CSetter<TSelf, TValue, TObject> && CProvider<TSelf, TValue, TObject>;
}
