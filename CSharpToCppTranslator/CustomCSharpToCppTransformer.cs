using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using Platform.RegularExpressions.Transformer;
using Platform.RegularExpressions.Transformer.CSharpToCpp;

namespace CSharpToCppTranslator
{
    public class CustomCSharpToCppTransformer : CSharpToCppTransformer
    {
        public static readonly IList<ISubstitutionRule> Rules = new List<SubstitutionRule>
        {
            // Just delete it in GenericCollectionMethodsBase.cs
            (new Regex(@"virtual TElement GetZero(.|\s)+Increment\(One\)(.|\s)+?}"), "", new Regex(@"GenericCollectionMethodsBase\.cs"), 0),
            // Just delete it in SizedBinaryTreeMethodsBase.cs
            (new Regex(@"\r?\n[\t ]+void FixSizes(.|\s)+};"), "    };", new Regex(@"SizedBinaryTreeMethodsBase\.cs"), 0),
            // Just delete it in SizedAndThreadedAVLBalancedTreeMethods.cs
            (new Regex(@"\r?\n[\t ]+void PrintNode(.|\s)+?}[\t ]*\r?\n"), "", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs"), 0),
            // TElement path[MaxPath] = { {0} }; 
            // TElement path[MaxPath]; path[0] = 0;
            (new Regex(@"TElement path\[([_a-zA-Z0-9]+)\] = \{ \{0\} \};"), "TElement path[$1]; path[0] = 0;", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs"), 0),
            // UncheckedConverter<TElement, long>.Default.Convert(node)
            // node
            (new Regex(@"UncheckedConverter<[a-zA-Z0-9]+, [a-zA-Z0-9]+>\.Default\.Convert\((?<argument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\)"), "${argument}", null, 0),
            // NumericType<TElement>.BytesSize
            // sizeof(TElement)
            (new Regex(@"NumericType<([a-zA-Z0-9]+)>\.BytesSize"), "sizeof($1)", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs"), 0),
            // EqualityComparer<TreeElement>.Default.Equals(this->GetElement(node), default)
            // iszero(GetElement(node), sizeof(TreeElement))
            (new Regex(@"EqualityComparer<TreeElement>\.Default\.Equals\(this->GetElement\(node\), default\)"), "iszero(this->GetElement(node), sizeof(TreeElement))", new Regex(@"Size[a-zA-Z]+Tree\.cs"), 0),
            // Zero
            // 0
            (new Regex(@"(\W)(Zero)(\W)"), "${1}0$3", null, 0),
            // One
            // 1
            (new Regex(@"(\W)(One)(\W)"), "${1}1$3", null, 0),
            // Two
            // 2
            (new Regex(@"(\W)(Two)(\W)"), "${1}2$3", null, 0),
            // Comparer.Compare(firstArgument, secondArgument) < 0
            // (firstArgument) < (secondArgument)
            (new Regex(@"(?<separator>\W)Comparer\.Compare\((?<firstArgument>((?<parenthesis>\()|(?<-parenthesis>\))|[^(),]*)+), (?<secondArgument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\) (?<operator>\S{1,2}) 0"), "${separator}(${firstArgument}) ${operator} (${secondArgument})", null, 0),
            // !this->AreEqual(firstArgument, secondArgument)
            // (firstArgument) != (secondArgument)
            (new Regex(@"(?<separator>\W)!(this->AreEqual|EqualityComparer\.Equals|EqualityComparer<[a-zA-Z0-9]+>\.Default\.Equals)\((?<firstArgument>((?<parenthesis>\()|(?<-parenthesis>\))|[^(),]*)+), (?<secondArgument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\)"), "${separator}(${firstArgument}) != (${secondArgument})", null, 0),
            // this->AreEqual(firstArgument, secondArgument)
            // (firstArgument) == (secondArgument)
            (new Regex(@"(?<separator>\W)(?<!::)(this->AreEqual|EqualityComparer\.Equals|EqualityComparer<[a-zA-Z0-9]+>\.Default\.Equals)\((?<firstArgument>((?<parenthesis>\()|(?<-parenthesis>\))|[^(),]*)+), (?<secondArgument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\)"), "${separator}(${firstArgument}) == (${secondArgument})", null, 0),
            // !this->EqualToZero(argument)
            // (argument) != 0
            (new Regex(@"(?<separator>\W)!this->EqualToZero\((?<argument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\)"), "${separator}(${argument}) != 0", null, 0),
            // this->EqualToZero(argument)
            // (argument) == 0
            (new Regex(@"(?<separator>\W)this->EqualToZero\((?<argument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\)"), "${separator}(${argument}) == 0", null, 0),
            // this->Add(firstArgument, secondArgument)
            // (firstArgument) + (secondArgument)
            (new Regex(@"(?<separator>\W)(Arithmetic\.Add|this->Add)\((?<firstArgument>((?<parenthesis>\()|(?<-parenthesis>\))|[^(),]*)+), (?<secondArgument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\)"), "${separator}(${firstArgument}) + (${secondArgument})", null, 0),
            // this->Increment(argument)
            // (argument) + 1
            (new Regex(@"(?<separator>\W)(Arithmetic\.Increment|this->Increment)\((?<argument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\)"), "${separator}(${argument}) + 1", null, 0),
            // this->Decrement(argument)
            // (argument) - 1;
            (new Regex(@"(?<separator>\W)(Arithmetic\.Decrement|this->Decrement)\((?<argument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\)"), "${separator}(${argument}) - 1", null, 0),
            // this->GreaterThan(firstArgument, secondArgument)
            // (firstArgument) > (secondArgument)
            (new Regex(@"(?<separator>\W)this->GreaterThan\((?<firstArgument>((?<parenthesis>\()|(?<-parenthesis>\))|[^(),]*)+), (?<secondArgument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\)"), "${separator}(${firstArgument}) > (${secondArgument})", null, 0),
            // this->GreaterThanZero(argument)
            // (argument) > 0
            (new Regex(@"(?<separator>\W)this->GreaterThanZero\((?<argument>((?<parenthesis>\()|(?<-parenthesis>\))|[^()]*)+)\)"), "${separator}(${argument}) > 0", null, 0),
            // template <typename TElement> class SizeBalancedTree : public SizeBalancedTreeMethods<TElement>
            // template <typename TElement, std::size_t N> class SizeBalancedTree : public Platform::Collections::Methods::Trees::SizeBalancedTreeMethods<TElement>
            (new Regex(@"template <typename TElement> class ([a-zA-Z0-9]+) : public ([a-zA-Z0-9]+)<TElement>"), "template <typename TElement, std::size_t N> class $1 : public Platform::Collections::Methods::Trees::$2<TElement>", new Regex(@"Size[a-zA-Z]+Tree\.cs"), 0),
            // SizeBalancedTree(int capacity) { (_elements, _allocated) = (new TreeElement[capacity], 1); }
            // SizeBalancedTree() { _allocated = 1; }
            (new Regex(@"([a-zA-Z0-9]+)\(int capacity\) { \(_elements, _allocated\) = \(new TreeElement\[capacity\], 1\); }"), "$1() { _allocated = 1; }", new Regex(@"Size[a-zA-Z]+Tree\.cs"), 0),
            // void PrintNodeValue(TElement node, StringBuilder sb) override { sb.Append(node); }
            //
            (new Regex(@"[\r\n]{1,2}\s+[\r\n]{1,2}\s+void PrintNodeValue\(TElement node, StringBuilder sb\) override { sb\.Append\(node\); }"), "", new Regex(@"Size[a-zA-Z]+Tree\.cs"), 0),
            // this->allocate(
            // allocate(
            (new Regex(@"this->(allocate|free|iszero)\("), "$1(", null, 0),
            // auto sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            // SizeBalancedTree<uint, 10000> sizeBalancedTree;
            (new Regex(@"auto ([a-zA-Z0-9]+) = new ([a-zA-Z0-9]+)<([_a-zA-Z0-9:]+)>\(([0-9]+)\);"), "$2<$3, $4> $1;", new Regex(@"TreesTests\.cs"), 0),
            // &sizeBalancedTree->Root
            // &sizeBalancedTree.Root
            (new Regex(@"&([a-zA-Z0-9]+)->([a-zA-Z0-9]+)"), "&$1.$2", new Regex(@"TreesTests\.cs"), 0),
            // sizeBalancedTree.Count
            // sizeBalancedTree.GetCount()
            (new Regex(@"([a-zA-Z0-9]+).Count"), "$1.GetCount()", new Regex(@"TreesTests\.cs"), 0),
            // sizeBalancedTree.Allocate
            // [&]()-> auto { return sizeBalancedTree.Allocate(); }
            (new Regex(@"(\(|, )([a-zA-Z0-9]+)\.(Allocate)"), "$1[&]()-> auto { return $2.$3(); }", new Regex(@"TreesTests\.cs"), 0),
            // sizeBalancedTree.Free
            // [&](std::uint32_t link)-> auto { sizeBalancedTree.Free(link); }
            (new Regex(@"(\(|, )([a-zA-Z0-9]+)\.(Free)"), "$1[&](std::uint32_t link)-> auto { $2.$3(link); }", new Regex(@"TreesTests\.cs"), 0),
            // sizeBalancedTree.TestMultipleCreationsAndDeletions(
            // TestExtensions::TestMultipleCreationsAndDeletions(sizeBalancedTree, 
            (new Regex(@"([a-zA-Z0-9]+)\.(TestMultipleCreationsAndDeletions|TestMultipleRandomCreationsAndDeletions)\("), "TestExtensions::$2<std::uint32_t>($1, ", new Regex(@"TreesTests\.cs"), 0),
            // SizedBinaryTreeMethodsBase
            // Platform::Collections::Methods::Trees::SizedBinaryTreeMethodsBase
            (new Regex(@"\(SizedBinaryTreeMethodsBase<TElement>"), "(Platform::Collections::Methods::Trees::SizedBinaryTreeMethodsBase<TElement>&", new Regex(@"TestExtensions\.cs"), 0),
        }.Cast<ISubstitutionRule>().ToList();

        public CustomCSharpToCppTransformer() : base(Rules) { }
    }
}
