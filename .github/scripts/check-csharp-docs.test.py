#!/usr/bin/env python3
"""Regression checks for generated C# XML documentation."""

from importlib.util import module_from_spec, spec_from_file_location
from pathlib import Path
from tempfile import TemporaryDirectory
import unittest


script = Path(__file__).with_name("check-csharp-docs.py")
spec = spec_from_file_location("check_csharp_docs", script)
module = module_from_spec(spec)
spec.loader.exec_module(module)


class DocumentationCoverageTests(unittest.TestCase):
    def validate(self, content: str) -> list[str]:
        with TemporaryDirectory() as directory:
            path = Path(directory) / "Platform.Interfaces.xml"
            path.write_text(content, encoding="utf-8")
            return module.validate_xml(path)

    def test_bilingual_member_and_parameter_are_accepted(self):
        xml = """<doc><members><member name="M:Example.Run(System.String)">
          <summary><para>Runs.</para><para>Запускает.</para></summary>
          <param name="input"><para>Input.</para><para>Ввод.</para></param>
        </member></members></doc>"""
        self.assertEqual(self.validate(xml), [])

    def test_missing_russian_summary_is_rejected(self):
        xml = """<doc><members><member name="T:Example">
          <summary><para>An example.</para></summary>
        </member></members></doc>"""
        self.assertTrue(any("summary" in error for error in self.validate(xml)))

    def test_missing_english_parameter_is_rejected(self):
        xml = """<doc><members><member name="M:Example.Run(System.String)">
          <summary><para>Runs.</para><para>Запускает.</para></summary>
          <param name="input"><para>Ввод.</para></param>
        </member></members></doc>"""
        self.assertTrue(any("param input" in error for error in self.validate(xml)))

    def test_empty_member_list_is_rejected(self):
        self.assertTrue(self.validate("<doc><members /></doc>"))


if __name__ == "__main__":
    unittest.main()
