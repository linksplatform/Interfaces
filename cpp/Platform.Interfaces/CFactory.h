#pragma once

namespace Platform::Interfaces
{
    template <typename TSelf, typename TProduct>
    concept CFactory = requires(TSelf self)
    {
        { self.Create() } -> std::same_as<TProduct>;
    };
}
