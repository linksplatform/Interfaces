#include <gtest/gtest.h>
#include <Platform.Interfaces.h>

namespace Platform::Interfaces::Tests
{
    struct EmptyICounter
    {
        int Count() { return {}; }
    };
    // You can use
    // static_assert(ICounter<EmptyICounter, int>);
    // to avoid mismatching the interface at compile time

    TEST(Tests, CompileTest)
    {
        ASSERT_TRUE((ICounter<EmptyICounter, int>));
        ASSERT_FALSE((ICounter<EmptyICounter, float>));
    }
}
