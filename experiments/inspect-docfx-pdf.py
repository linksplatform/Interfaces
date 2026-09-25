"""Inspect a generated DocFX PDF with: pip install pypdf."""

import sys

from pypdf import PdfReader


reader = PdfReader(sys.argv[1])
text = "\n".join(page.extract_text() or "" for page in reader.pages)

print(f"Pages: {len(reader.pages)}")
for topic in ("Interfaces", "Platform.Interfaces", "IFactory", "IProvider"):
    print(f"{topic}: {topic in text}")

if len(reader.pages) < 2 or any(topic not in text for topic in ("IFactory", "IProvider")):
    raise SystemExit("The PDF is missing expected API content.")
