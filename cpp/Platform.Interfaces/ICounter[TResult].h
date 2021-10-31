#pragma once

namespace Platform::Interfaces
{
    template <typename ...>
    struct ICounter;
    
    template <typename TResult>
    struct ICounter<TResult>
    {
        virtual TResult Count() = 0;
        
        virtual ~ICounter() = default;
    };
}
