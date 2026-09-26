#!/usr/bin/env bash

set -euo pipefail

repository_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
output_directory=${1:-"$repository_root/csharp/_site"}
work_directory=$(mktemp -d)

cleanup() {
  rm -rf "$work_directory"
}
trap cleanup EXIT

sudo apt-get update &
update_pid=$!
wait "$update_pid"
sudo apt-get install -y \
  ghostscript \
  python3-pygments \
  texlive \
  texlive-lang-cyrillic \
  texlive-latex-extra &
install_pid=$!
wait "$install_pid"

document="$work_directory/document.tex"
cat > "$document" <<'LATEX'
\documentclass[11pt,a4paper,fleqn]{report}
\usepackage[left=5mm,top=5mm,right=5mm,bottom=5mm]{geometry}
\usepackage[T1]{fontenc}
\usepackage[T2A]{fontenc}
\usepackage[utf8]{inputenc}
\usepackage{fvextra}
\usepackage{minted}
\usemintedstyle{vs}
\usepackage{makeidx}
\usepackage[columns=1]{idxlayout}
\usepackage[tiny]{titlesec}
\makeindex
\titlespacing\chapter{0mm}{0mm}{0mm}
\titlespacing\section{0mm}{0mm}{0mm}
\DeclareUnicodeCharacter{221E}{\ensuremath{\infty}}
\DeclareUnicodeCharacter{FFFD}{\ensuremath{ }}
\begin{document}
\sffamily
\chapter*{LinksPlatform's Platform.Interfaces Class Library}
LATEX

while IFS= read -r -d '' source_file; do
  relative_path=${source_file#"$repository_root/"}
  latex_path=${relative_path//_/\\_}
  {
    printf '\\index{%s}\n' "$latex_path"
    printf '\\section{%s}\n' "$latex_path"
    printf '%s\n' '\begin{minted}[tabsize=2,breaklines,breakanywhere,linenos=true,xleftmargin=7mm,framesep=4mm]{csharp}'
    sed $'1s/^\xEF\xBB\xBF//' "$source_file"
    printf '%s\n' '\end{minted}'
  } >> "$document"
done < <(
  find \
    "$repository_root/csharp/Platform.Interfaces" \
    "$repository_root/csharp/Platform.Interfaces.Tests" \
    -type f -name '*.cs' ! -path '*/bin/*' ! -path '*/obj/*' -print0 |
    sort -z
)

cat >> "$document" <<'LATEX'
\printindex
\end{document}
LATEX

(
  cd "$work_directory"
  pdflatex -shell-escape -interaction=nonstopmode -halt-on-error document.tex
  makeindex document.idx
  pdflatex -shell-escape -interaction=nonstopmode -halt-on-error document.tex
)

mkdir -p "$output_directory"
cp "$work_directory/document.pdf" "$output_directory/Platform.Interfaces.pdf"
echo "Generated $output_directory/Platform.Interfaces.pdf"
