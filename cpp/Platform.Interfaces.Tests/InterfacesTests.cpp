namespace Platform::Interfaces::Tests
{
    TEST_CLASS(Interfaces)
    {
        public: TEST_METHOD(BuildTest)
        {
            ICounter<int, int>* c1;
            ICounter<int>* c2;
            ICriterionMatcher<int>* cm1;
            IFactory<int>* f1;
            IProperties<int, int, int>* p1;
            IProperty<int, int>* p2;
            IProvider<int, int>* p3;
            IProvider<int>* p4;
            ISetter<int, int>* s1;
            ISetter<int>* s2;
        }
    };
}
