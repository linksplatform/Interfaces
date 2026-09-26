#!/usr/bin/env python3
"""Require bilingual documentation on public C++ declarations."""

from pathlib import Path
import re
import sys


def declaration_kind(line: str) -> str | None:
    stripped = line.strip()
    if re.match(r"(?:concept|struct|class)\s+\w+", stripped) and ("{" in stripped or "=" in stripped):
        return "type"
    if re.match(r"using\s+\w+\s*=", stripped):
        return "alias"
    if stripped.startswith("virtual "):
        return "method"
    if stripped.startswith("#define "):
        return "macro"
    if re.match(r"(?:THIS_REFERENCE_WRAPPER_METHODS|VARIABLE_WRAPPER_METHODS|USE_ALL_BASE_CONSTRUCTORS)\(", stripped):
        return "generated members"
    if re.match(r"ExtendedReferenceBase\(", stripped):
        return "constructor"
    if re.match(r"TExtendable&?\s+extendable;", stripped):
        return "field"
    return None


def documentation_before(lines: list[str], index: int) -> tuple[str, str]:
    previous = index - 1
    template = ""
    if previous >= 0 and lines[previous].strip().startswith("template <"):
        template = lines[previous].strip()
        previous -= 1
    comment = []
    while previous >= 0 and lines[previous].lstrip().startswith("///"):
        comment.append(lines[previous].strip())
        previous -= 1
    return "\n".join(reversed(comment)), template


def summary_errors(comment: str, label: str) -> list[str]:
    errors = []
    if "<summary>" not in comment or "</summary>" not in comment:
        return [f"{label} lacks a documentation summary"]
    paragraphs = re.findall(r"<para>(.*?)</para>", comment)
    if not any(re.search(r"[A-Za-z]", paragraph) for paragraph in paragraphs) or not any(
        re.search(r"[А-Яа-яЁё]", paragraph) for paragraph in paragraphs
    ):
        errors.append(f"{label} lacks English and Russian paragraphs")
    return errors


def template_errors(comment: str, template: str, label: str) -> list[str]:
    parameters = dict.fromkeys(re.findall(r"\bT(?:[A-Z]\w*)?\b", template))
    return [
        f"{label} lacks documentation for {parameter}"
        for parameter in parameters
        if f'<typeparam name="{parameter}">' not in comment
    ]


def callable_errors(comment: str, signature: str, kind: str, label: str) -> list[str]:
    errors = []
    arguments = re.search(r"\((.*?)\)", signature)
    if arguments:
        for argument in arguments.group(1).split(","):
            name = re.search(r"(\w+)$", argument.strip())
            if name and f'<param name="{name.group(1)}">' not in comment:
                errors.append(f"{label} lacks documentation for argument {name.group(1)}")
    if kind == "method" and not ("virtual void " in signature or "virtual ~" in signature) and "<returns>" not in comment:
        errors.append(f"{label} lacks return documentation")
    return errors


def declaration_errors(lines: list[str], index: int, kind: str, path: Path) -> list[str]:
    comment, template = documentation_before(lines, index)
    label = f"{path}:{index + 1}: {kind}"
    errors = summary_errors(comment, label)
    if kind == "type":
        errors.extend(template_errors(comment, template, label))
    if kind in {"method", "constructor"}:
        errors.extend(callable_errors(comment, lines[index].strip(), kind, label))
    return errors


def validate_header(path: Path) -> list[str]:
    lines = path.read_text(encoding="utf-8-sig").splitlines()
    errors = []
    inside_internal = False
    for index, line in enumerate(lines):
        if "namespace Internal {" in line:
            inside_internal = True
        if "}  // namespace Internal" in line:
            inside_internal = False
            continue
        if inside_internal:
            continue
        kind = declaration_kind(line)
        if kind is not None:
            errors.extend(declaration_errors(lines, index, kind, path))
    return errors


def main() -> int:
    root = Path(__file__).resolve().parents[2] / "cpp" / "Platform.Interfaces"
    errors = [error for path in sorted(root.glob("*.h")) for error in validate_header(path)]
    if errors:
        print("\n".join(errors), file=sys.stderr)
        return 1
    print(f"Bilingual documentation covers the public declarations in {len(list(root.glob('*.h')))} C++ headers.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
