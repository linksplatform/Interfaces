#pragma once

#include "Macros.h"

namespace Platform::Interfaces
{
    template<typename TExtendable, template<typename, typename> typename TFirstExtender, template<typename, typename> typename ...TExtenders>
    struct Facaded : public FacadedIntermediate<Facaded<TExtendable, TFirstExtender, TExtenders...>, TExtendable, TFirstExtender, TExtenders...>
    {
        using base = FacadedIntermediate<Facaded<TExtendable, TFirstExtender, TExtenders...>, TExtendable, TFirstExtender, TExtenders...>;

        USE_ALL_BASE_CONSTRUCTORS(Facaded, base)
    };
}
