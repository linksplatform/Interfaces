#pragma once

namespace Platform::Interfaces
{
    template <typename ...> struct ISetter;
    template <typename TValue, typename TArgument> struct ISetter<TValue, TArgument>
    {
        virtual void Set(TArgument argument, TValue value) = 0;
        
        virtual ~ISetter<TValue, TArgument>() = default;
    };
}
