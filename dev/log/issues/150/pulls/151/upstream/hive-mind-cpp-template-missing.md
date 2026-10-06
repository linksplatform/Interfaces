## Summary

`fix --ci-cd` never recommends [link-foundation/cpp-ai-driven-development-pipeline-template](https://github.com/link-foundation/cpp-ai-driven-development-pipeline-template), and `docs/CI-CD-BEST-PRACTICES.md` does not list it. The template was created on 2026-09-27, but `CI_CD_TEMPLATES` in `src/fix.ci-cd.lib.mjs` still has only JS, Rust, Python, Go, C#, Java and PHP. C and C++ repositories therefore get no C++ template, and C++ shows up under "Other detected languages without a dedicated template".

## Where it happened

[linksplatform/Interfaces#150](https://github.com/linksplatform/Interfaces/issues/150) was created by `fix --ci-cd`. C++ is that repository's main language: 80,749 of its 106,360 bytes. The issue still lists only the C#, JS and Python templates, and ends with:

> Other detected languages without a dedicated template: C++, C, CMake, Shell.

The maintainer had to point the solver to the C++ template by hand ([linksplatform/Interfaces#151 comment](https://github.com/linksplatform/Interfaces/pull/151#issuecomment-6024565555)).

## Reproduction

Run this against hive-mind `03dcb2e` (main, 2026-10-06), with the languages from `gh api repos/linksplatform/Interfaces/languages`:

```js
// node repro.mjs <hive-mind checkout> <languages.json>
import { readFileSync } from "node:fs";
import { pathToFileURL } from "node:url";
const [root, languagesFile] = process.argv.slice(2);
const lib = await import(pathToFileURL(`${root}/src/fix.ci-cd.lib.mjs`).href);
const languages = JSON.parse(readFileSync(languagesFile, "utf8"));
const { sortedTemplates, unmatchedLanguages } = lib.mapLanguagesToTemplates(languages);
console.log("templates:", sortedTemplates.map((entry) => entry.template.repo));
console.log("unmatched:", unmatchedLanguages);
```

The languages input is `{"C++":80749,"C#":15034,"JavaScript":6203,"C":1553,"CMake":1391,"Shell":914,"Python":516}`. The output is:

```text
templates: [
  'link-foundation/csharp-ai-driven-development-pipeline-template',
  'link-foundation/js-ai-driven-development-pipeline-template',
  'link-foundation/python-ai-driven-development-pipeline-template'
]
unmatched: [ 'C++', 'C', 'CMake', 'Shell' ]
```

## Workaround

Name the template in the issue or in a PR comment, as was done on linksplatform/Interfaces#151.

## Suggested fix

1. Add an entry to `CI_CD_TEMPLATES` in `src/fix.ci-cd.lib.mjs`. The template's description is "A template for AI driven development pipeline for C/C++", and it builds with CMake:

   ```diff
      {
   +    key: 'cpp',
   +    label: 'C / C++',
   +    languages: ['C++', 'C', 'CMake'],
   +    repo: 'link-foundation/cpp-ai-driven-development-pipeline-template',
   +  },
   +  {
        key: 'php',
   ```

   With this entry the same input returns the C++ template first, and only `Shell` is unmatched. `node tests/test-fix-ci-cd.mjs` still reports "38 passed, 0 failed".
2. Add a test like the existing "CI_CD_TEMPLATES includes the PHP template (issue #1733)".
3. Add the C++ row to both template tables in `docs/CI-CD-BEST-PRACTICES.md` ("Recommended CI/CD Templates" and "Language → Template Mapping"). Do the same in the `.ru`, `.zh` and `.hi` translations.
4. Optionally add a check that compares `CI_CD_TEMPLATES` with the `*-ai-driven-development-pipeline-template` repositories in link-foundation (`gh repo list link-foundation`). A new template then cannot be missed again. Today `cpp` is the only one of the 8 that is missing.

The reproduction log, languages file and diff are in [linksplatform/Interfaces `dev/log/issues/150/pulls/151/upstream/`](https://github.com/linksplatform/Interfaces/tree/issue-150-dfc4758d0022/dev/log/issues/150/pulls/151/upstream).
