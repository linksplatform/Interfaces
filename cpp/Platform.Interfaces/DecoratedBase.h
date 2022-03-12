#pragma once

#include "Macros.h"

namespace Platform::Interfaces
{
    template<typename TFacade, typename TDecorated, template<typename, typename> typename TFirstDecorator, template<typename, typename> typename ...TDecorators>
    struct DecoratedBase : public DECORATED_BASE_TYPE(DecoratedBase, TFacade, TDecorated, TFirstDecorator, TDecorators)
    {
        using base = DECORATED_BASE_TYPE(DecoratedBase, TFacade, TDecorated, TFirstDecorator, TDecorators);

        USE_ALL_BASE_CONSTRUCTORS(DecoratedBase, base)
    };
}
