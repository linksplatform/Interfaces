#!/usr/bin/env python3
"""Regression checks for C++ documentation coverage."""

from importlib.util import module_from_spec, spec_from_file_location
from pathlib import Path
from tempfile import TemporaryDirectory
import unittest


script = Path(__file__).with_name("check-cpp-docs.py")
spec = spec_from_file_location("check_cpp_docs", script)
module = module_from_spec(spec)
spec.loader.exec_module(module)


class DocumentationCoverageTests(unittest.TestCase):
    def validate(self, source: str) -> list[str]:
        with TemporaryDirectory() as directory:
            path = Path(directory) / "Test.h"
            path.write_text(source)
            return module.validate_header(path)

    def test_undocumented_concept_is_rejected(self):
        self.assertTrue(self.validate("template <typename TSelf>\nconcept CExample = true;\n"))

    def test_bilingual_concept_with_type_parameter_is_accepted(self):
        source = """/// <summary>
/// <para>Checks an example.</para>
/// <para>Проверяет пример.</para>
/// </summary>
/// <typeparam name="TSelf">The type.</typeparam>
template <typename TSelf>
concept CExample = true;
"""
        self.assertEqual(self.validate(source), [])

    def test_undocumented_public_member_is_rejected(self):
        source = """/// <summary>
/// <para>A holder.</para>
/// <para>Хранилище.</para>
/// </summary>
struct Holder {
  using Value = int;
};
"""
        self.assertTrue(any("alias" in error for error in self.validate(source)))


if __name__ == "__main__":
    unittest.main()
