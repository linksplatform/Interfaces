#pragma once

namespace Platform::Interfaces
{
    template <typename ...>
    struct ICli;

    struct ICli
    {
        virtual void Run(TArgument argument) = 0;

        virtual ~ICli() = default;
    };
}
