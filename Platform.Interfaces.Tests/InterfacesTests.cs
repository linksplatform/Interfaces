using Xunit;

#pragma warning disable CS0168 // Variable is declared but never used

namespace Platform.Interfaces.Tests
{
    public static class InterfacesTests
    {
        [Fact]
        public static void BuildTest()
        {
            ICounter<int, int> c1;
            ICounter<int> c2;
            ICriterionMatcher<int> cm1;
            IFactory<int> f1;
            IProperties<int, int, int> p1;
            IProperty<int, int> p2;
            IProvider<int, int> p3;
            IProvider<int> p4;
            ISetter<int, int> s1;
            ISetter<int> s2;
        }
    }
}
