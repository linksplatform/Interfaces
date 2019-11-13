using System;
using Platform.RegularExpressions.Transformer;

namespace CSharpToCppTranslator
{
    public class Program
    {
        static int Main(string[] args)
        {
            //for (int i = 0; i < args.Length; i++)
            //{
            //    Console.WriteLine(args[i]);
            //}
            //args = new string[] { @"C:\Code\Links\Collections.Methods\Platform.Collections.Methods\GenericCollectionMethodsBase.cs", @"C:\Code\Links\Collections.Methods\cpp\Platform.Collections.Methods\GenericCollectionMethodsBase.cpp" };
            var csharpToCpp = new CustomCSharpToCppTransformer();
            var cli = new TransformerCLI(csharpToCpp);
            var success = cli.Run(args, out string message);
            if (!string.IsNullOrWhiteSpace(message))
            {
                Console.WriteLine(message);
            }
            return success ? 0 : -1;
        }
    }
}
