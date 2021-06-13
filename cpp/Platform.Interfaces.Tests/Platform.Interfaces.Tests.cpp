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
        ICounter<int> auto counter = EmptyICounter{};

        ASSERT_TRUE((ICounter<EmptyICounter, int>));
        ASSERT_FALSE((ICounter<EmptyICounter, float>));

        ASSERT_TRUE((ICounter<decltype(counter), int>));
        ASSERT_FALSE((ICounter<decltype(counter), float>));
    }
}
