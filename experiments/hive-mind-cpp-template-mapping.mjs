// Shows which CI/CD templates Hive Mind's `fix --ci-cd` recommends for a
// repository. Without a C++ entry in CI_CD_TEMPLATES, linksplatform/Interfaces
// (mostly C++) gets no C++ template, which is why issue #150 did not list
// link-foundation/cpp-ai-driven-development-pipeline-template.
// Usage: node experiments/hive-mind-cpp-template-mapping.mjs <hive-mind checkout> "$(cat <languages.json>)"
import { pathToFileURL } from "node:url";
const [root, languagesJson] = process.argv.slice(2);
const lib = await import(pathToFileURL(`${root}/src/fix.ci-cd.lib.mjs`).href);
const languages = JSON.parse(languagesJson);
const { sortedTemplates, unmatchedLanguages } = lib.mapLanguagesToTemplates(languages);
console.log("templates:", sortedTemplates.map((entry) => entry.template.repo));
console.log("unmatched:", unmatchedLanguages);
