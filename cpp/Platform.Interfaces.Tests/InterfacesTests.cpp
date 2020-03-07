namespace Platform::Interfaces::Tests
{
    TEST_CLASS(Interfaces)
    {
        public: TEST_METHOD(BuildTest)
        {
            ICounter<std::int32_t, std::int32_t> c1 = nullptr;
            ICounter<std::int32_t> c2 = nullptr;
            ICriterionMatcher<std::int32_t> cm1 = nullptr;
            IFactory<std::int32_t> f1 = nullptr;
            IProperties<std::int32_t, std::int32_t, std::int32_t> p1 = nullptr;
            IProperty<std::int32_t, std::int32_t> p2 = nullptr;
            IProvider<std::int32_t, std::int32_t> p3 = nullptr;
            IProvider<std::int32_t> p4 = nullptr;
            ISetter<std::int32_t, std::int32_t> s1 = nullptr;
            ISetter<std::int32_t> s2 = nullptr;
        }
    };
}
