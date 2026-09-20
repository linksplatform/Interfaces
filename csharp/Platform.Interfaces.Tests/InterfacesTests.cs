using System.IO;
using System.Reflection.PortableExecutable;
using Xunit;

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

            Assert.Null(c1);
            Assert.Null(c2);
            Assert.Null(cm1);
            Assert.Null(f1);
            Assert.Null(p1);
            Assert.Null(p2);
            Assert.Null(p3);
            Assert.Null(p4);
            Assert.Null(s1);
            Assert.Null(s2);
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
