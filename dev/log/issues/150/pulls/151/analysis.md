# Issue #150: false positives, false negatives, warnings and errors in CI/CD

This is the case study for [issue #150](https://github.com/linksplatform/Interfaces/issues/150) and [PR #151](https://github.com/linksplatform/Interfaces/pull/151).

Everything cited below lives in this folder:

| Path | Contents |
|---|---|
| `issue-150.json`, `pr-151.json` | The issue and the pull request, as fetched |
| `runs-list.json` | The last 100 runs on `main` before the fix |
| `ci-logs/run-<id>.log` + `.json` | Full logs and metadata of every run the issue lists, plus the earlier failing runs of the same workflows |
| `ci-logs/annotations.txt` | Check-run annotations (errors, warnings, notices) of those runs |
| `ci-logs-after/` | Logs, job lists and annotations of the runs for `c77f922`, the first commit with all the fixes |
| `lint/*-before.txt`, `lint/*-after.txt` | actionlint and zizmor results before and after |
| `lint/lychee-offline-probe.txt` | Why no link checker was added (see "Template comparison") |
| `nuget-apt-probe.log` | `apt-get install nuget` in a clean `ubuntu:24.04` container |
| `cpp-false-positive-experiment.log` | The C++ test job passing with a failing test |
| `nuget-verification-key-probe.log` | The NuGet API-key probe against nuget.org, and the preflight failing on the expired key |
| `cpp-sanitizers-experiment.log` | Memory and overflow errors that pass the plain build and fail the sanitized one |
| `cpp-clang-format-before.log` | The 56 clang-format violations before the headers were formatted |
| `ci-logs-after/run-375*-cpp-matrix.log` | The C++ matrix (gcc, clang, sanitizers, AppleClang, MSVC) passing |
| `cpp-template/` | Comparison with the C++ template (`comparison.md`), plus its CI logs, annotations and CodeQL alerts |
| `upstream/` | Bodies and evidence of the upstream issues filed |

## Timeline

| Date (UTC) | Event | Evidence |
|---|---|---|
| 2021-08-19 | `DEPENDABOT_AUTO_MERGE_TOKEN` secret last updated | `gh api repos/linksplatform/Interfaces/actions/secrets` |
| 2022-12-02 | `NUGET_TOKEN` secret last updated. nuget.org API keys expire after at most 365 days | same |
| 2022-12-13 | `Platform.Interfaces.TemplateLibrary` 0.3.41 is the last C++ version on nuget.org | [flat-container index](https://api.nuget.org/v3-flatcontainer/platform.interfaces.templatelibrary/index.json) |
| 2022-12-14 | `Platform.Interfaces` 0.5.0 is the last C# version on nuget.org | [flat-container index](https://api.nuget.org/v3-flatcontainer/platform.interfaces/index.json) |
| 2025-09-12 | Release `csharp_0.5.1` is created, but 0.5.1 is not on nuget.org. The old workflow created releases without a successful push | `gh release list` |
| 2025-09-19 | GitHub deprecates Node.js 20 on Actions runners | [changelog](https://github.blog/changelog/2025-09-19-deprecation-of-node-20-on-github-actions-runners/) |
| 2026-01-27 | GitHub removes the `@dependabot merge` / `squash and merge` comment commands that `AutoMerge.yml` relied on | [changelog](https://github.blog/changelog/2026-01-27-changes-to-github-dependabot-pull-request-comment-commands/) |
| 2026-09-21 | Release `cpp_0.3.43` is created without a NuGet package | `gh release list` |
| 2026-09-25 20:41 | csharp run [36187216094](https://github.com/linksplatform/Interfaces/actions/runs/36187216094): `pushToNuget` returns 403 | `ci-logs/run-36187216094.log:792` |
| 2026-09-26 02:14 | csharp run [36210988617](https://github.com/linksplatform/Interfaces/actions/runs/36210988617) returns 403 again. Deploy cpp run [36210989177](https://github.com/linksplatform/Interfaces/actions/runs/36210989177) fails with exit 100, yet creates release `cpp_0.4.0` | `run-36210988617.log:792`, `run-36210989177.log:2600` |
| 2026-09-26 02:54 | Merge of #137 (`3dfbee6`). csharp [36213154898](https://github.com/linksplatform/Interfaces/actions/runs/36213154898) returns 403. Deploy cpp [36213155523](https://github.com/linksplatform/Interfaces/actions/runs/36213155523) fails with exit 100 and creates release `cpp_0.4.1`. The other five runs are green, but they carry warnings and notices | `annotations.txt` |
| 2026-10-04 | Scheduled CodeQL [37194208178](https://github.com/linksplatform/Interfaces/actions/runs/37194208178) passes | |
| 2026-10-06 19:11 | Issue #150 opened. Draft PR #151 created at 19:12 | `issue-150.json`, `pr-151.json` |
| 2026-10-06 19:40 | `c77f922`: all 6 workflows are green with **zero annotations**, and the C++ job runs 8 tests | `ci-logs-after/` |
| 2026-10-06 20:08 | konard: "Also check for cpp version" ([cpp-ai-driven-development-pipeline-template](https://github.com/link-foundation/cpp-ai-driven-development-pipeline-template)), "Do the same for it as in #150" | PR #151 comments |
| 2026-10-06 20:26 | `8ff0b25`: the C++ tests pass on GNU 13.3.0, Clang 18.1.3, AppleClang 17.0.0, MSVC 19.51 and under ASan+UBSan. The macOS job logs pipx's setuptools warning | `ci-logs-after/run-37526407726-cpp-matrix.log:1514` |
| 2026-10-06 20:31 | `cb02abf`: the pipx warning is gone. The only annotation left is GitHub's macOS arm64 capacity notice | `ci-logs-after/run-37527025000-cpp-matrix.log` |
| 2026-10-06 20:46 | `1c4ba2d`: all green. Two log-only warnings are left: pipx's DeprecationWarning in the new format job, and CodeQL's "No NuGet feeds are reachable" | `ci-logs-after/1c4ba2d/` |
| 2026-10-06 20:59 | `35f8162`: the pipx warning is gone. The root `nuget.config` (`83d7023`) did not remove CodeQL's feed warning | `ci-logs-after/35f8162/run-37530537674.log` |
| 2026-10-06 21:05 | `4f6ff2c`: the CodeQL feed check is off and its warning is gone. A scan of all logs for `warn`, `deprecat` and `error` finds Conan's `profile detect` advice (row 29) and zizmor's offline fallback (row 30) | `ci-logs-after/4f6ff2c/` |

## Inventory of problems

**Classes:**

- **FP**: false positive. The run is green, or a release appears, although the thing it reports did not happen.
- **FN**: false negative. A real problem is not reported, or a working path is never exercised.
- **E**: error.
- **W**: warning or notice.

| # | Class | Workflow | Symptom | Root cause | Fix in this PR |
|---|---|---|---|---|---|
| 1 | E | csharp | `pushToNuget`: `403 (The specified API key is invalid, has expired, or does not have permission…)` (`run-36213154898.log:1060`) | The `NUGET_TOKEN` secret dates from 2022-12-02. nuget.org keys last at most 365 days | `preflight-nuget-release.mjs` reports `publish_required` from the flat-container index and the auth mode (`trusted-publishing` / `api-key` / `none`). With an API key, it also asks nuget.org whether the key can push the package (row 24). `NuGet/login` (OIDC) is used when `vars.NUGET_USER` is set, with the secret as a fallback. `push-nuget-package.sh` turns a 403 into instructions. **The secret itself must still be renewed or replaced by a maintainer.** |
| 2 | E | Deploy new cpp version | `E: Package 'nuget' has no installation candidate`, exit 100 (`run-36213155523.log:2600-2605`) | `ubuntu-latest` became Ubuntu 24.04, which dropped the `nuget` apt package (reproduced in `nuget-apt-probe.log`) | `pack-cpp-nuget.mjs` packs the `.nuspec` with `dotnet pack` and verifies the package. The push uses `dotnet nuget push`. Both run from local workflows instead of `linksplatform/Workflows@main` |
| 3 | FP | Deploy new cpp version | Releases `cpp_0.3.43`, `cpp_0.4.0` and `cpp_0.4.1` exist, but no such package is on nuget.org | The upstream `publishRelease` job `needs: [test, get_package_info, get_conan_package_info]`. It does not depend on the push | `publishRelease` has `needs: [test, pushToNuget]` and no `if:`. It skips when the tag already exists. A policy test enforces this |
| 4 | FP | Test cpp | Green although a failing test would also be green (`cpp-false-positive-experiment.log`) | The upstream `cpp-test.yml` ran `cmake --build .` and never `ctest`. gtest 1.10 (`cci.20210126`) also emitted Conan 1 deprecation warnings | `cpp/build-and-test.sh` runs `ctest --output-on-failure --no-tests=error`. CMake uses `enable_testing()` and `gtest_discover_tests()`. gtest is 1.18.0. Conan is pinned to 2.33.0 |
| 5 | FN | Test cpp | PR commits never started the tests | `pull_request: types: [edited]` reacts only to title and body edits | Default PR activity types. The path filters cover all of `cpp/**`. A policy test bans `edited` |
| 6 | FN | auto-merge | Every Dependabot PR was merged by hand. The run always shows `skipped` | (a) It posted `@dependabot merge` comments, which were removed on 2026-01-27. (b) Its `github.actor` check is spoofable and is skipped on re-runs. (c) The repository has `allow_auto_merge=false`, so `gh pr merge --auto` could not work either | `dependabot/fetch-metadata` (hash-pinned) plus `gh pr merge --auto --merge`, gated on `pull_request.user.login`. The job fails with an `::error` that names the setting while `allow_auto_merge` is off, or while the token is missing. **A maintainer must enable "Allow auto-merge".** |
| 7 | W | C++ documentation, Deploy new cpp version | `Node.js 20 is deprecated … actions/upload-artifact@v4`, `… ncipollo/release-action@v1.11.2` | Node 20 actions | `upload-artifact@v7`, `download-artifact@v8` and `gh release create`. A policy test bans Node 20 majors and the removed actions |
| 8 | W | every workflow | `The ubuntu-latest label will migrate to Ubuntu 26 beginning October 19, 2026` ([runner-images#14748](https://github.com/actions/runner-images/issues/14748)) | Floating runner label. This same kind of migration caused #2 | `ubuntu-24.04` everywhere. A policy test enforces it |
| 9 | W | csharp | `warn : No API Key was provided … nuget.pkg.github.com` (3 occurrences) | The key was only given to `dotnet nuget add source` | `--api-key "$GITHUB_TOKEN"` on the push. GitHub Packages has no trusted publishing, so zizmor's informational `use-trusted-publishing` finding is suppressed inline, with the reason |
| 10 | W | checkout (all) | `hint: Using 'master' as the name for the initial branch` | Git's default-branch hint | `GIT_CONFIG_*` env trio in every workflow, the same pattern as the C# template |
| 11 | FN | all | Nothing linted the workflows. zizmor: **70 findings, 30 high** (`lint/zizmor-before.txt`) | No workflow lint job | New `Workflows` workflow: actionlint (Docker image, includes shellcheck, pinned by digest), zizmor 1.29.0 through `zizmor-action` (annotations), plus a pedantic high/high pass. Repository policy tests. Result: 0 findings in every mode (`lint/*-after.txt`) |
| 12 | FN | csharp | `tj-actions/changed-files` (compromised in [CVE-2025-30066](https://github.com/advisories/GHSA-mrrh-fwg8-r2c3)) decided whether docs were rebuilt | Third-party action with a mutable tag | `git diff --name-only` against the push or PR base, with a `git cat-file -e` fallback |
| 13 | FN | csharp | Re-running a release for a version that is already published would fail with a duplicate. An unpublished version was never retried | No version check before publishing | The preflight reads the nuget.org flat-container index. 0.6.1 is unpublished, so it will publish once credentials work |
| 14 | W/FN | all | No `permissions:`, no `timeout-minutes`, persisted checkout credentials, `always()` gates, `@main` references | Hardening gaps (zizmor `excessive-permissions`, `artipacked`) | Least-privilege `permissions` (top level and per job), a timeout on every job, `persist-credentials: false`, `!cancelled()`. All in-repo. Enforced by `workflow-policy.test.mjs` |
| 15 | FN | all | Stale PR runs kept running. No secrets scan | Missing concurrency control. No secret detection ([best practices](https://github.com/link-assistant/hive-mind/blob/main/docs/CI-CD-BEST-PRACTICES.md) §10, §11) | Concurrency in every workflow: cancelled on PRs, never on `main`, so writers are not interrupted. A `Secrets` workflow (secretlint 13.0.7, pinned). `experiments/secretlint-detects-planted-secret.sh` proves the scan can fail |
| 16 | — | codeql | Dependencies and workflows were not covered | — | CodeQL `actions` language and `dependency-review-action@v5` on PRs (`fail-on-severity: high`) |
| 17 | — | dependabot | Updates landed the day they were published | — | `cooldown: default-days: 7`, the same as the templates |
| 18 | FP | Codacy (GitHub App) | `action_required`: "18 new issues (0 max.)" on this PR, all markdownlint, in the case-study files (`codacy/pr-151-new-issues-before.json`) | MD034, MD040 and MD041 were genuine. MD043 (required heading structure) is enabled in Codacy without a heading list, so it reports `Expected: [None]` for every file with a heading, including `README.md:11` and `MIGRATION_SUMMARY.md:1` on `main` (`codacy/main-md043.json`) | Bare URLs wrapped, fence languages added, top-level headings added; `markdownlint-cli2` with Codacy's enabled rules reports 0 errors. `.codacy.yml` excludes the archived evidence under `dev/log/`, which is not code. **A maintainer should disable MD043 in Codacy's code patterns** |
| 19 | FN | Test cpp | Only GCC on Linux was tested, though the headers are consumed by MSVC (NuGet) and AppleClang/Clang (Conan) | One-job workflow | Matrix: linux-gcc, linux-clang, linux-gcc-sanitizers, macos-appleclang, windows-msvc, with `fail-fast: false`. `build-and-test.sh` passes `--config`/`--build-config` for MSVC's multi-config generator |
| 20 | FN | Test cpp | Undefined behaviour and memory errors in tests passed (`cpp-sanitizers-experiment.log`) | No sanitizers | ASan+UBSan build with `-fno-sanitize-recover=all` |
| 21 | FN | `cpp/CMakeLists.txt` | `LINKS_PLATFORM_TESTS` was forced to `TRUE`, so the option did nothing. The `armv7`/`armv8` `-march` branches never matched (Linux reports `armv7l`/`aarch64`) | Dead configuration | Cache option defaulting to `OFF`. `LINKS_PLATFORM_EXTRA_FLAGS` for opt-in flags. Policy test |
| 22 | FN | Deploy new cpp version | A package that packs but cannot be consumed would be published | Nothing restored the package | `check-cpp-nuget-package.sh` restores it with `dotnet restore`, checks `build/native/*.targets`, and compiles against the headers. Its test shows it fails on broken packages |
| 23 | FN | Test cpp | `.clang-format` existed but was never enforced: 56 violations in 7 headers (`cpp-clang-format-before.log`) | No format check | `format` job: `clang-format==23.1.3`, `--dry-run --Werror`. The headers are formatted (whitespace only) |
| 24 | FP | csharp, Deploy new cpp version | The preflight could not tell whether `NUGET_TOKEN` worked; the 403 appeared only at push time | Its comment claimed nuget.org has no read-only key check | `verifyApiKey`: `create-verification-key`, then `verifykey` against the newest published version. On the real gallery, the expired key now fails the preflight (`nuget-verification-key-probe.log`). Without a published version, the scope is reported as unknown, not as valid |
| 25 | W | Test cpp (macOS) | `WARNING: Skipping setuptools as it is not installed.` | pipx 1.17 always runs `pip uninstall setuptools` when it creates its shared venv. Python 3.12+ venvs have no setuptools (`upstream/pipx-setuptools-warning-*.log`) | Exactly that line is filtered from `pipx install`'s stderr. Reported as [pypa/pipx#2068](https://github.com/pypa/pipx/issues/2068) |
| 26 | W | Test cpp (macOS) | Notice: "Due to capacity constraints, jobs targeting macOS arm64 runners may experience longer queue times." | GitHub platform notice for macOS arm64 runners | **Not fixable in the repository.** It is informational; no runner label avoids it except dropping macOS, which would lose AppleClang coverage |
| 27 | W | Test cpp `format` | `<string>:1: DeprecationWarning: Implicit None on return values is deprecated and will raise KeyErrors.` (`ci-logs-after/1c4ba2d/run-37528763604.log`) | The Ubuntu image ships pipx 1.16.7, which reads `metadata['Requires-Python']`. clang-format's wheel declares none, so Python 3.12's `importlib.metadata` warns. pipx 1.17 uses `.get()`, so it is fixed upstream (`pipx-1.16-requires-python-warning.log`) | The install step sets `PYTHONWARNINGS` to ignore exactly that message. Once the image ships pipx 1.17, the filter does nothing |
| 28 | W | CodeQL (csharp) | `[build-stdout] Warning: No NuGet feeds are reachable.` in every C# analysis, including runs before this PR (`ci-logs/run-36213154910.log`) | CodeQL's buildless restore checks the feeds from the repository's `nuget.config` files. With no such file, it checks an empty set, and `GetReachableNuGetFeeds` warns on any empty result (`FeedManager.cs`, CodeQL 2.27.1 and `main`). The explicit and inherited feed sets are checked separately, so with only nuget.org one of them is always empty | A root `nuget.config` was tried first (`83d7023`). It only moved the empty set from explicit to inherited feeds, and the warning stayed (`ci-logs-after/35f8162/run-37530537674.log`), so it was reverted. The analyze step now sets `CODEQL_EXTRACTOR_CSHARP_BUILDLESS_NUGET_FEEDS_CHECK=false`. nuget.org is the only feed, so CodeQL restores from it as before, without the probe. A policy test pins it. Reported as [github/codeql#22766](https://github.com/github/codeql/issues/22766) |
| 29 | W | Test cpp (all 5 jobs) | `WARN: This profile is a guess of your environment, please check it.`, `WARN: The output of this command is not guaranteed to be stable and can change in future Conan versions.`, `WARN: Use your own profile files for stability.`, and on macOS `WARN: Defaulted to cppstd='gnu17' for apple-clang.` (`ci-logs-after/4f6ff2c/run-37531301224.log`) | `cpp/build-and-test.sh` creates the default profile with `conan profile detect`, which always ends with this advice (`conan/cli/commands/profile.py`, Conan 2.33.0). The warnings have no tag, so `core:skip_warnings` cannot select them. The detected profile is what CI wants: each matrix job tests the compiler the runner has, and `conan install` sets `compiler.cppstd=20` | Exactly those four lines are filtered from `conan profile detect`'s stderr; any other warning still reaches the log. `cpp-build-and-test.test.mjs` runs the script with a stub `conan` and fails without the filter. Not reported: Conan prints the advice on purpose |
| 30 | W | Workflows `zizmor` | `WARN audit: zizmor: zizmor is running in offline mode by default; some audits and auto-fixes will not be available.` (`ci-logs-after/4f6ff2c/run-37531301179.log:484`) | The pedantic pass runs `pipx run zizmor` without a token, so zizmor falls back to offline mode and says so. The zizmor action step before it runs the online audits (`online-audits: true`, line 359) | The pedantic pass passes `--offline`, which zizmor accepts silently. A policy test requires it |

## Requirements of the issue and their status

| Requirement | Status |
|---|---|
| Find and fix all false positives, false negatives, warnings and errors | Done, see the table above. After the fix, CI for `c77f922` and later commits is green with no annotations and no `##[warning]` lines (`ci-logs-after/annotations.txt` is empty) |
| Keep logs and data in `dev/log/issues/150/pulls/151` | Done |
| Deep analysis with timeline, root causes and solutions | This document |
| Add debug output, off by default, where root causes are unclear | `RUNNER_DEBUG=1` prints the changed-file list in `findChangedCsFiles`. `VERBOSE=1` makes the release scripts print the HTTP lookups and decisions |
| Report upstream issues with reproduction, workaround and fix suggestion | [linksplatform/Workflows#6](https://github.com/linksplatform/Workflows/issues/6) (apt `nuget` on 24.04, and the release created without a package), [#7](https://github.com/linksplatform/Workflows/issues/7) (tests never run), [#8](https://github.com/linksplatform/Workflows/issues/8) (Node 20, `ubuntu-latest`, `@main`, 114 zizmor findings), [csharp template#66](https://github.com/link-foundation/csharp-ai-driven-development-pipeline-template/issues/66) (no NuGet trusted publishing; [key verification comment](https://github.com/link-foundation/csharp-ai-driven-development-pipeline-template/issues/66#issuecomment-6025071528)), [#67](https://github.com/link-foundation/csharp-ai-driven-development-pipeline-template/issues/67) and [python template#94](https://github.com/link-foundation/python-ai-driven-development-pipeline-template/issues/94) (Git hint in three workflows), [C++ template#5](https://github.com/link-foundation/cpp-ai-driven-development-pipeline-template/issues/5) (`-latest` labels, open CodeQL alerts, Git hint), [C++ template#6](https://github.com/link-foundation/cpp-ai-driven-development-pipeline-template/issues/6) (NuGet preflight false positive, no trusted publishing), [pypa/pipx#2068](https://github.com/pypa/pipx/issues/2068) (setuptools warning), [github/codeql#22766](https://github.com/github/codeql/issues/22766) (empty feed set warning) |
| Apply each fix everywhere | Policy tests check every workflow for each rule, not just the one where it was found |
| Compare with the three templates and reuse their best practices | See below |
| PR comment: do the same for the C++ template | Done: `cpp-template/comparison.md` compares every file. Rows 19-27 above. Two issues were filed upstream |
| Follow `CI-CD-BEST-PRACTICES.md` | See below |
| Version bump if needed | Not needed. C++ 0.4.1 and C# 0.6.1 are both still unpublished on nuget.org, so the first successful run publishes them. `publishRelease` skips the existing `cpp_0.4.1` tag |

## Template comparison

All files in the three templates were compared at `csharp@22e53c8`, `js` and `python` (HEAD of 2026-10-06).

| Template practice | Here |
|---|---|
| `workflows.yml` running actionlint and zizmor, with `.github/zizmor.yml` | Adopted. The policy pins `actions/*` and `github/*` by ref, and every other action by hash (including `zizmorcore/zizmor-action`, after CodeQL `actions/unpinned-tag` flagged its tag ref on this PR) |
| `security.yml`: CodeQL including `actions`, plus dependency review | Adopted in `codeql.yml`. The file name is kept for the README badge |
| Release preflight before publishing (csharp #51/#57, best practices §16) | Adopted. It also checks the registry, so a published version is skipped |
| Trusted publishing (npm in js, PyPI in python) | Adopted for NuGet. The csharp template lacks it, so it was reported as [#66](https://github.com/link-foundation/csharp-ai-driven-development-pipeline-template/issues/66) |
| `ubuntu-24.04`, `timeout-minutes`, `persist-credentials: false`, least privilege, `GIT_CONFIG_*` env | Adopted in every workflow and enforced by tests |
| Concurrency (`cancel-in-progress: ${{ github.ref != 'refs/heads/main' }}`) | Adopted |
| secretlint with `.secretlintrc.json` (js, python) | Adopted, as a separate `Secrets` workflow |
| Policy tests for workflows (`*-workflow-policy.test.mjs`) | Adopted, plus repository-wide `workflow-policy.test.mjs` |
| Dependabot `cooldown` | Adopted |
| `links.yml` (lychee) | **Not adopted, deliberately.** The repository tracks two Markdown files. All 52 of their links are external, and an offline lychee run has nothing to check (`lint/lychee-offline-probe.txt`). An online check would add exactly the kind of flaky false positives this issue is about. The csharp template needs a web-archive recheck script to contain them. Internal workflow links are already verified by `check-readme-badges.mjs` |
| C++ template (`cpp-template/comparison.md`) | Compiler matrix, sanitizers, consumer check of the NuGet package, pinned clang-format, and the NuGet key probe (completed with the `verifykey` step) were adopted. Changelog fragments, version bumping, coverage, clang-tidy/cppcheck and vcpkg/Conan packaging were not; the reasons are listed per file |
| Changesets, version bump scripts and file-size limits, plus formatter checks and multi-OS matrices outside C++ | Out of scope. They change the release model and the language toolchains, not the correctness of CI. The issue asks to fix CI problems, and none of the problems found stems from their absence |

## CI-CD-BEST-PRACTICES.md compliance

| § | Topic | Status |
|---|---|---|
| 1 | Run checks only on relevant changes | `paths:` filters. The changed-file detection uses `git diff` |
| 4 | Static analysis | CodeQL (c-cpp, csharp, actions), actionlint, zizmor |
| 5 | Fast-fail ordering | The preflight runs before publishing jobs. Release jobs `need` the tests |
| 9 | Release automation | Version-aware, idempotent publish. The release is created only after the push |
| 10 | Concurrency control | Read-only work is cancelled on PRs. `main` is never cancelled. `deploy-cpp` is never cancelled |
| 11 | Secrets detection | `Secrets` workflow |
| 14 | Lint the workflows | `Workflows` workflow. actionlint runs as the Docker image (with shellcheck) |
| 15 | Audit dependencies | dependency review on PRs. The CodeQL schedule stays weekly |
| 16 | Prove you can publish before you build | Preflight, trusted publishing, 403 guidance |

## Actions only a maintainer can take

1. **NuGet credentials.** Choose one:
   - Configure [trusted publishing](https://learn.microsoft.com/nuget/nuget-org/trusted-publishing) on nuget.org for `linksplatform/Interfaces`, for the workflows `csharp.yml` and `deploy-cpp.yml`. Then set the repository variable `NUGET_USER` to the nuget.org profile name.
   - Or renew the `NUGET_TOKEN` secret with push rights for `Platform.Interfaces` and `Platform.Interfaces.TemplateLibrary`.

   After that, re-run `csharp` and `Deploy new cpp version` on `main`, or push. 0.6.1 and 0.4.1 will be published.
2. **Dependabot auto-merge.** Enable *Settings → General → Allow auto-merge*. Check that `DEPENDABOT_AUTO_MERGE_TOKEN` (last updated 2021-08-19) is still valid. Until then, the auto-merge job fails with an error that names the missing setting, instead of being silently skipped.
3. **Releases without packages.** `cpp_0.3.43`, `cpp_0.4.0` and `csharp_0.5.1` point to NuGet versions that do not exist. They were left untouched, because deleting releases cannot be undone. Delete them, or mark them in their notes.
4. **Codacy MD043.** Disable markdownlint's *MD043 (required heading structure)* in Codacy's code patterns, or give it a heading list. As configured, it flags every new markdown file that has a heading.

## Verification

- `node --test .github/scripts/*.test.mjs`: 102 tests, 0 failures. The new policy assertions were mutation-tested: removing a `concurrency:` block, or making a writer workflow cancellable, makes them fail.
- The Python documentation tests pass. `check-readme-badges.mjs` passes.
- actionlint 1.7.12: exit 0. zizmor 1.29.0 offline, online and pedantic high/high: "No findings to report".
- `experiments/cpp-false-positive-test-run.sh`: a failing gtest now fails `cpp/build-and-test.sh`.
- `experiments/secretlint-detects-planted-secret.sh`: secretlint detects a planted token.
- CI on the PR: all workflows are green. Test cpp logs `100% tests passed, 0 tests failed out of 8` in each of the 5 matrix jobs. The only annotation is GitHub's macOS capacity notice (row 26). After rows 27 to 30, a scan of the logs for `warn` and `deprecat` finds only the lines the steps filter and the macOS notice. CodeQL's C# extractor also logs `Failed to resolve 1 types in 0 namespaces`, as it did before this PR (`ci-logs/run-36213154910.log`). It is an extraction statistic, not a warning, and the analysis completes; which type it is was not investigated.
- `experiments/cpp-sanitizers-catch-errors.sh`: the sanitized build fails on a heap overflow and a signed overflow that the plain build passes.
- `check-cpp-nuget-package.test.mjs`: the consumer check fails without the MSBuild targets or with a header missing.
- `.github/scripts/check-cpp-format.sh` with clang-format 23.1.3: 42 files formatted. Before the formatting commit it failed with 56 errors.

## References

- NuGet trusted publishing: <https://learn.microsoft.com/nuget/nuget-org/trusted-publishing>, and `NuGet/login`: <https://github.com/NuGet/login>
- Node 20 deprecation: <https://github.blog/changelog/2025-09-19-deprecation-of-node-20-on-github-actions-runners/>
- Dependabot comment commands removal: <https://github.blog/changelog/2026-01-27-changes-to-github-dependabot-pull-request-comment-commands/>
- Ubuntu 26 migration of `ubuntu-latest`: <https://github.com/actions/runner-images/issues/14748>
- tj-actions/changed-files compromise: CVE-2025-30066, <https://github.com/advisories/GHSA-mrrh-fwg8-r2c3>
- zizmor audits: <https://docs.zizmor.sh/audits/>; actionlint: <https://github.com/rhysd/actionlint>
- Auto-merge setting: <https://docs.github.com/repositories/configuring-branches-and-merges-in-your-repository/configuring-pull-request-merges/managing-auto-merge-for-pull-requests-in-your-repository>
