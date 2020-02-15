namespace Platform::Interfaces::Tests
{
    TEST_CLASS(Interfaces)
    {
        public: TEST_METHOD(BuildTest)
        {
            ICounter<std::int32_t, std::int32_t>* c1;
            ICounter<std::int32_t>* c2;
            ICriterionMatcher<std::int32_t>* cm1;
            IFactory<std::int32_t>* f1;
            IProperties<std::int32_t, std::int32_t, std::int32_t>* p1;
            IProperty<std::int32_t, std::int32_t>* p2;
            IProvider<std::int32_t, std::int32_t>* p3;
            IProvider<std::int32_t>* p4;
            ISetter<std::int32_t, std::int32_t>* s1;
            ISetter<std::int32_t>* s2;
        }
    };
}
