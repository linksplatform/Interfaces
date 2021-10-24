#pragma once

#include "ISetter[TValue, TArgument].h"
#include "IProvider[TProvided, TArgument].h"

namespace Platform::Interfaces
{
    template <typename ...> struct IProperty;
    template <typename TObject, typename TValue> struct IProperty<TObject, TValue> : public ISetter<TValue, TObject>, IProvider<TValue, TObject>
    {
        virtual ~IProperty<TObject, TValue>() = default;
    };
}
