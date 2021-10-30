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
        struct EmptyProperties: public IProperties<int, int, int>
        {
            void SetValue(int, int, int) {}

            int GetValue(int, int) { return {}; }
        };
        static_assert(CProperties<EmptyProperties, int, int, int>);

        {
            CProperties<int, int, int> auto properties = EmptyProperties{};

            ASSERT_TRUE((CProperties<EmptyProperties, int, int, int>));
            ASSERT_TRUE((CProperties<EmptyProperties, int&, int, int>));
            ASSERT_TRUE((CProperties<EmptyProperties, const int&, int, int>));
        }
    }

    TEST(CompileTests, Provider)
    {
        struct EmptyProvider: public IProvider<int>
        {
            int Get() { return {}; }
        };
        static_assert(CProvider<EmptyProvider, int>);

        struct EmptyIntProvider: public IProvider<int, int>
        {
            int Get(int argument) { return {}; }
        };
        static_assert(CProvider<EmptyIntProvider, int, int>);

        {
            CProvider<int> auto provider = EmptyProvider{};

            ASSERT_TRUE((CProvider<EmptyProvider, int>));
        }

        {
            CProvider<int, int> auto provider = EmptyIntProvider{};

            ASSERT_TRUE((CProvider<EmptyIntProvider, int, int>));
            ASSERT_TRUE((CProvider<EmptyIntProvider, int, float>));
        }
    }

    TEST(CompileTests, Setter)
    {
        struct EmptySetter: public ISetter<int>
        {
            void Set(int value) { }
        };
        static_assert(CSetter<EmptySetter, int>);

        struct EmptyIntSetter: public ISetter<int, int>
        {
            void Set(int argument, int value) {  }
        };
        static_assert(CSetter<EmptyIntSetter, int, int>);

        {
            CSetter<int> auto provider = EmptySetter{};

            ASSERT_TRUE((CSetter<EmptySetter, int>));
        }

        {
            CSetter<int, int> auto provider = EmptyIntSetter{};

            ASSERT_TRUE((CSetter<EmptyIntSetter, int, int>));
            ASSERT_TRUE((CSetter<EmptyIntSetter, int, float>));
        }
    }

    TEST(CompileTests, Property)
    {
        struct EmptyProperty: public IProperty<int&, int> 
        {
            void Set(int& object, int value) { }
            int Get(int& object) { return {}; }
        };
        // TODO can be use in concepts TObject& instead of TObject
        static_assert(CProperty<EmptyProperty, int&, int>);

        {
            CProperty<int&, int> auto provider = EmptyProperty{};

            ASSERT_TRUE((CProperty<EmptyProperty, int&, int>));
            ASSERT_TRUE((CProvider<EmptyProperty, int, int&>));
            ASSERT_TRUE((CSetter<EmptyProperty, int, int&>));
        }
    }
}
