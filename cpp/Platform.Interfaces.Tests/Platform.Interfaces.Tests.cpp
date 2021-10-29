#include <gtest/gtest.h>
#include <Platform.Interfaces.h>

namespace Platform::Interfaces::Tests
{
    TEST(CompileTests, Counter)
    {
        struct EmptyCounter: ICounter<int>
        {
            int Count() { return {}; }
        };
        // You can use
        static_assert(CCounter<EmptyCounter, int>);
        // to avoid mismatching the interface at compile time

        struct EmptyCounterInt: public ICounter<int, int>
        {
            int Count(int argument) { return {}; }
        };
        static_assert(CCounter<EmptyCounterInt, int, int>);

        {
            CCounter<int> auto counter = EmptyCounter{};

            ASSERT_TRUE((CCounter<EmptyCounter, int>));
            ASSERT_FALSE((CCounter<EmptyCounter, float>));
        }

        {
            CCounter<int, int> auto counter = EmptyCounterInt{};

            ASSERT_TRUE((CCounter<EmptyCounterInt, int, int>));
            ASSERT_TRUE((CCounter<EmptyCounterInt, int, float>));
            ASSERT_FALSE((CCounter<EmptyCounterInt, float, int>));
            ASSERT_FALSE((CCounter<EmptyCounterInt, float, float>));
        }
    }

    TEST(CompileTests, CriterionMatcher)
    {
        struct EmptyCriterionMatcher: public ICriterionMatcher<int>
        {
            bool IsMatched(int argument) { return {}; }
        };
        static_assert(CCriterionMatcher<EmptyCriterionMatcher, int>);

        {
            CCriterionMatcher<int> auto criterionMatcher = EmptyCriterionMatcher{};

            ASSERT_TRUE((CCriterionMatcher<EmptyCriterionMatcher, int>));
            ASSERT_TRUE((CCriterionMatcher<EmptyCriterionMatcher, float>));
        }
    }

    TEST(CompileTests, Factory)
    {
        struct EmptyIntFactory: public IFactory<int>
        {
            int Create() { return {}; }
        };
        static_assert(CFactory<EmptyIntFactory, int>);

        {
            CFactory<int> auto factory = EmptyIntFactory{};

            ASSERT_TRUE((CFactory<EmptyIntFactory, int>));
            ASSERT_FALSE((CFactory<EmptyIntFactory, float>));
        }
    }

    TEST(CompileTests, Properties)
    {
        struct EmptyProperties: public IProperties<std::string, int, std::byte>
        {
            //SetValue
        };
        //static_assert(CProperties<EmptyProperties, std::string, int, std::byte>);

        {
            //CProperties<std::string, int, std::byte> auto properties = EmptyProperties{};

            ASSERT_TRUE((CProperties<EmptyProperties, std::string, int, std::byte>));
            ASSERT_TRUE((CProperties<EmptyProperties, std::string&, int, std::byte>));
            ASSERT_TRUE((CProperties<EmptyProperties, const std::string&, int, std::byte>));
        }
    }

    TEST(CompileTests, Provider)
    {
    struct EmptyProvider: public IProvider<std::byte>
        {
            std::byte Get() { return std::byte{}; }
        };
        static_assert(CProvider<EmptyProvider, std::byte>);

    struct EmptyIntProvider: public IProvider<std::byte, int>
        {
            std::byte Get(int argument) { return std::byte{}; }
        };
        static_assert(CProvider<EmptyIntProvider, std::byte, int>);

        {
            CProvider<std::byte> auto provider = EmptyProvider{};

            ASSERT_TRUE((CProvider<EmptyProvider, std::byte>));
        }

        {
            CProvider<std::byte, int> auto provider = EmptyIntProvider{};

            ASSERT_TRUE((CProvider<EmptyIntProvider, std::byte, int>));
            ASSERT_TRUE((CProvider<EmptyIntProvider, std::byte, float>));
        }
    }

    TEST(CompileTests, Setter)
    {
        struct EmptySetter: public ISetter<std::byte>
        {
            void Set(std::byte value) { }
        };
        static_assert(CSetter<EmptySetter, std::byte>);

        struct EmptyIntSetter: public ISetter<std::byte, int>
        {
            void Set(int argument, std::byte value) {  }
        };
        static_assert(CSetter<EmptyIntSetter, std::byte, int>);

        {
            CSetter<std::byte> auto provider = EmptySetter{};

            ASSERT_TRUE((CSetter<EmptySetter, std::byte>));
        }

        {
            CSetter<std::byte, int> auto provider = EmptyIntSetter{};

            ASSERT_TRUE((CSetter<EmptyIntSetter, std::byte, int>));
            ASSERT_TRUE((CSetter<EmptyIntSetter, std::byte, float>));
        }
    }

    TEST(CompileTests, Property)
    {
        struct EmptyProperty: public IProperty<std::string&, std::byte> 
        {
            void Set(std::string& object, std::byte value) { }
            std::byte Get(std::string& object) { return {}; }
        };
        // TODO can be use in concepts TObject& instead of TObject
        static_assert(CProperty<EmptyProperty, std::string&, std::byte>);

        {
            CProperty<std::string&, std::byte> auto provider = EmptyProperty{};

            ASSERT_TRUE((CProperty<EmptyProperty, std::string&, std::byte>));
            ASSERT_TRUE((CProvider<EmptyProperty, std::byte, std::string&>));
            ASSERT_TRUE((CSetter<EmptyProperty, std::byte, std::string&>));
        }
    }
}
