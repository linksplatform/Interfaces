#include <gtest/gtest.h>
#include <Platform.Interfaces.h>

namespace Platform::Interfaces::Tests
{
    TEST(CompileTests, ICounter)
    {
        struct EmptyCounter
        {
            int Count() { return {}; }
        };
        // You can use
        static_assert(ICounter<EmptyCounter, int>);
        // to avoid mismatching the interface at compile time

        struct EmptyCounterInt
        {
            int Count(int argument) { return {}; }
        };
        static_assert(ICounter<EmptyCounterInt, int, int>);

        {
            ICounter<int> auto counter = EmptyCounter{};

            ASSERT_TRUE((ICounter<EmptyCounter, int>));
            ASSERT_FALSE((ICounter<EmptyCounter, float>));
        }

        {
            ICounter<int, int> auto counter = EmptyCounterInt{};

            ASSERT_TRUE((ICounter<EmptyCounterInt, int, int>));
            ASSERT_TRUE((ICounter<EmptyCounterInt, int, float>));
            ASSERT_FALSE((ICounter<EmptyCounterInt, float, int>));
            ASSERT_FALSE((ICounter<EmptyCounterInt, float, float>));
        }
    }

    TEST(CompileTests, ICriterionMatcher)
    {
        struct EmptyCriterionMatcher
        {
            bool IsMatched(int argument) { return {}; }
        };
        static_assert(ICriterionMatcher<EmptyCriterionMatcher, int>);

        {
            ICriterionMatcher<int> auto criterionMatcher = EmptyCriterionMatcher{};

            ASSERT_TRUE((ICriterionMatcher<EmptyCriterionMatcher, int>));
            ASSERT_TRUE((ICriterionMatcher<EmptyCriterionMatcher, float>));
        }
    }

    TEST(CompileTests, IFactory)
    {
        struct EmptyIntFactory
        {
            int Create() { return {}; }
        };
        static_assert(IFactory<EmptyIntFactory, int>);

        {
            IFactory<int> auto factory = EmptyIntFactory{};

            ASSERT_TRUE((IFactory<EmptyIntFactory, int>));
            ASSERT_FALSE((IFactory<EmptyIntFactory, float>));
        }
    }

    TEST(CompileTests, IProperties)
    {
        struct EmptyProperties
        {
            void SetValue(std::string object, int argument, std::byte value) {}

            std::byte GetValue(const std::string& object, int argument) { return std::byte{}; }
        };
        static_assert(IProperties<EmptyProperties, std::string, int, std::byte>);

        {
            IProperties<std::string, int, std::byte> auto properties = EmptyProperties{};

            ASSERT_TRUE((IProperties<EmptyProperties, std::string, int, std::byte>));
            ASSERT_TRUE((IProperties<EmptyProperties, std::string&, int, std::byte>));
            ASSERT_TRUE((IProperties<EmptyProperties, const std::string&, int, std::byte>));
        }
    }

    TEST(CompileTests, IProvider)
    {
        struct EmptyProvider
        {
            std::byte Get() { return std::byte{}; }
        };
        static_assert(IProvider<EmptyProvider, std::byte>);

        struct EmptyIntProvider
        {
            std::byte Get(int argument) { return std::byte{}; }
        };
        static_assert(IProvider<EmptyIntProvider, std::byte, int>);

        {
            IProvider<std::byte> auto provider = EmptyProvider{};

            ASSERT_TRUE((IProvider<EmptyProvider, std::byte>));
        }

        {
            IProvider<std::byte, int> auto provider = EmptyIntProvider{};

            ASSERT_TRUE((IProvider<EmptyIntProvider, std::byte, int>));
            ASSERT_TRUE((IProvider<EmptyIntProvider, std::byte, float>));
        }
    }

    TEST(CompileTests, ISetter)
    {
        struct EmptySetter
        {
            void Set(std::byte value) { }
        };
        static_assert(ISetter<EmptySetter, std::byte>);

        struct EmptyIntSetter
        {
            void Set(int argument, std::byte value) {  }
        };
        static_assert(ISetter<EmptyIntSetter, std::byte, int>);

        {
            ISetter<std::byte> auto provider = EmptySetter{};

            ASSERT_TRUE((ISetter<EmptySetter, std::byte>));
        }

        {
            ISetter<std::byte, int> auto provider = EmptyIntSetter{};

            ASSERT_TRUE((ISetter<EmptyIntSetter, std::byte, int>));
            ASSERT_TRUE((ISetter<EmptyIntSetter, std::byte, float>));
        }
    }

    TEST(CompileTests, IProperty)
    {
        struct EmptyProperty
        {
            void Set(std::string& object, std::byte value) { }
            std::byte Get(std::string& object) { return {}; }
        };
        // TODO can be use in concepts TObject& instead of TObject
        static_assert(IProperty<EmptyProperty, std::string&, std::byte>);

        {
            IProperty<std::string&, std::byte> auto provider = EmptyProperty{};

            ASSERT_TRUE((IProperty<EmptyProperty, std::string&, std::byte>));
            ASSERT_TRUE((IProvider<EmptyProperty, std::byte, std::string&>));
            ASSERT_TRUE((ISetter<EmptyProperty, std::byte, std::string&>));
        }
    }
}
