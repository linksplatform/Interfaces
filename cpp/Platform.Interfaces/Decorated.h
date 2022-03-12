#pragma once

#include "Macros.h"

namespace Platform::Interfaces
{
    template<typename TDecorated, template<typename, typename> typename TFirstDecorator, template<typename, typename> typename ...TDecorators>
    struct Decorated : public DecoratedIntermediate<Decorated<TDecorated, TFirstDecorator, TDecorators...>, TDecorated, TFirstDecorator, TDecorators...>
    {
        using base = DecoratedIntermediate<Decorated<TDecorated, TFirstDecorator, TDecorators...>, TDecorated, TFirstDecorator, TDecorators...>;

        USE_ALL_BASE_CONSTRUCTORS(Decorated, base)
    };
}
