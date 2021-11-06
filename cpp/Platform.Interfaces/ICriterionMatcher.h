#pragma once

namespace Platform::Interfaces
{
    template <typename ...>
    struct ICriterionMatcher;
    
    template <typename TArgument> 
    struct ICriterionMatcher<TArgument>
    {
        virtual bool IsMatched(TArgument argument) = 0;
        
        virtual ~ICriterionMatcher() = default;
    };
}
