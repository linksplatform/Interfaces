using System.IO;
using System.Reflection.PortableExecutable;
using Xunit;

#pragma warning disable CS0168 // Variable is declared but never used
#pragma warning disable CS0219 // Variable is assigned but its value is never used

namespace Platform.Interfaces.Tests
{
    public static class InterfacesTests
    {
        [Fact]
        public static void BuildTest()
        {
            ICounter<int, int>? c1 = null;
            ICounter<int>? c2 = null;
            ICriterionMatcher<int>? cm1 = null;
            IFactory<int>? f1 = null;
            IProperties<int, int, int>? p1 = null;
            IProperty<int, int>? p2 = null;
            IProvider<int, int>? p3 = null;
            IProvider<int>? p4 = null;
            ISetter<int, int>? s1 = null;
            ISetter<int>? s2 = null;
        }

        [Fact]
        public static void AssemblyContainsEmbeddedPortablePdb()
        {
            using var assembly = File.OpenRead(typeof(ICounter<>).Assembly.Location);
            using var peReader = new PEReader(assembly);

            Assert.Contains(
                peReader.ReadDebugDirectory(),
                entry => entry.Type == DebugDirectoryEntryType.EmbeddedPortablePdb);
        }
    }
}
