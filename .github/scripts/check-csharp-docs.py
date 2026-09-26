#!/usr/bin/env python3
"""Require English and Russian paragraphs in generated C# API documentation."""

from pathlib import Path
import re
import sys
import xml.etree.ElementTree as ET


DOCUMENTATION_TAGS = {"summary", "remarks", "typeparam", "param", "returns", "value", "example", "exception"}


def has_bilingual_paragraphs(element: ET.Element) -> bool:
    paragraphs = ["".join(paragraph.itertext()) for paragraph in element.findall("para")]
    return any(re.search(r"[A-Za-z]", paragraph) for paragraph in paragraphs) and any(
        re.search(r"[А-Яа-яЁё]", paragraph) for paragraph in paragraphs
    )


def validate_xml(path: Path) -> list[str]:
    root = ET.parse(path).getroot()
    members = root.findall("./members/member")
    if not members:
        return [f"{path}: generated XML contains no documented API members"]

    errors = []
    for member in members:
        name = member.get("name", "unnamed member")
        if member.find("summary") is None:
            errors.append(f"{name}: missing summary")
        for section in member:
            if section.tag not in DOCUMENTATION_TAGS:
                continue
            label = f"{section.tag} {section.get('name', '')}".strip()
            if not has_bilingual_paragraphs(section):
                errors.append(f"{name}: {label} lacks English and Russian paragraphs")
    return errors


def main() -> int:
    if len(sys.argv) != 2:
        print("Usage: check-csharp-docs.py XML_FILE", file=sys.stderr)
        return 2
    path = Path(sys.argv[1])
    try:
        errors = validate_xml(path)
    except (OSError, ET.ParseError) as error:
        print(f"{path}: {error}", file=sys.stderr)
        return 1
    if errors:
        print("\n".join(errors), file=sys.stderr)
        return 1
    count = len(ET.parse(path).getroot().findall("./members/member"))
    print(f"Validated bilingual XML documentation for {count} C# API members.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
