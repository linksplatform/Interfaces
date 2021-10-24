#pragma once

namespace Platform::Interfaces
{
    template <typename ...> struct IProperties;
    template <typename TObject, typename TProperty, typename TValue> struct IProperties<TObject, TProperty, TValue>
    {
        virtual TValue GetValue(TObject object, TProperty property) = 0;

        virtual void SetValue(TObject object, TProperty property, TValue value) = 0;
        
        virtual ~IProperties<TObject, TProperty, TValue>() = default;
    };
}
