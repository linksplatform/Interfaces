#pragma once

namespace Platform::Interfaces
{
    template <typename TSelf, typename TObject, typename TProperty, typename TValue>
    concept CProperties = requires(TSelf self, TObject object, TProperty property, TValue value)
    {
        { self.GetValue(object, property) } -> std::same_as<TValue>;

        { self.SetValue(object, property, value) } -> std::same_as<void>;
    };
}
