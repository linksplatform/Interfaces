# Comparison with the C++ template

This compares the repository with [link-foundation/cpp-ai-driven-development-pipeline-template](https://github.com/link-foundation/cpp-ai-driven-development-pipeline-template) at `16bac38` (2026-09-30, the HEAD of `main` on 2026-10-06), file by file. konard asked for it on PR #151.

The template's CI logs and annotations are in `ci-logs/`, and its code-scanning alerts in `codeql-alerts.json`.

## Adopted

| Template file | Practice | Here |
|---|---|---|
| `release.yml` `test` matrix | gcc and clang on Linux, AppleClang on macOS, MSVC on Windows, `fail-fast: false` | `cpp-test.yml` `test` matrix. The concurrency group includes `matrix.name`, so the jobs do not cancel each other. Run 37526407726: GNU 13.3.0, Clang 18.1.3, AppleClang 17.0.0 and MSVC 19.51 all pass the 8 tests |
| `release.yml` `sanitizers` | ASan + UBSan with `-fno-sanitize-recover=all` | `linux-gcc-sanitizers` matrix entry, `LINKS_PLATFORM_SANITIZERS` in `cpp/CMakeLists.txt`. `experiments/cpp-sanitizers-catch-errors.sh` shows a heap overflow and a signed overflow that pass the plain build and fail the sanitized one (`../cpp-sanitizers-experiment.log`) |
| `CMakeLists.txt`, `cmake/ProjectOptions.cmake` | Tests are an option that the build turns on, not a forced setting; no hard-coded `-march` | `LINKS_PLATFORM_TESTS` defaults to `OFF` and `cpp/build-and-test.sh` passes `ON`. The `armv7`/`armv8` `-march` branches, which never matched Linux's `armv7l`/`aarch64`, became the opt-in `LINKS_PLATFORM_EXTRA_FLAGS` |
| `CMakePresets.json`, `release.yml` | Multi-config generators get `--config` / `--build-config` | `cpp/build-and-test.sh`. MSVC builds and runs the tests in the chosen configuration |
| `scripts/check-nuget-package.sh`, `release.yml` `packages` job | Restore the packed NuGet package like a consumer and compile against it before publishing | `.github/scripts/check-cpp-nuget-package.sh`, run by the `pack` job. `check-cpp-nuget-package.test.mjs` shows it fails without `build/native/*.targets` or without a header |
| `scripts/lint_cpp.py` (`format`), `scripts/requirements-ci.txt` | `clang-format --dry-run --Werror` with a version pinned through PyPI | `.github/scripts/check-cpp-format.sh`, `format` job in `cpp-test.yml`, `clang-format==23.1.3`. Before, 7 headers had 56 violations (`../cpp-clang-format-before.log`) |
| `scripts/preflight-credentials.sh` | Probe the NuGet key with `create-verification-key` before releasing | `preflight-nuget-release.mjs` `verifyApiKey`. It also runs the `verifykey` step, because `create-verification-key` does not check the package id (reported upstream) |
| `Doxyfile` | `WARN_IF_UNDOCUMENTED`, `WARN_AS_ERROR = FAIL_ON_WARNINGS` | Already in `cpp/Doxyfile` |
| `.github/workflows/workflows.yml`, `.github/zizmor.yml` | actionlint and zizmor | Adopted earlier in this PR from the C#/JS/Python templates. Here every non-`actions/*`/`github/*` action is hash-pinned, which avoids the template's two open CodeQL alerts |
| `security.yml` | CodeQL including `actions`, dependency review | `codeql.yml` |
| `.secretlintrc.json` | secretlint | `secrets.yml` |
| `.github/dependabot.yml` | `cooldown` | Adopted |
| all workflows | `timeout-minutes`, `persist-credentials: false`, least privilege, concurrency | Adopted and enforced by `workflow-policy.test.mjs` |

## Not adopted, and why

| Template file | Reason |
|---|---|
| Runner labels `ubuntu-latest`, `macos-latest`, `windows-latest` | Floating labels change the OS under an unchanged commit, which is how `ubuntu-latest` → 24.04 broke every C++ deploy here. Pinned to `ubuntu-24.04`, `macos-15` and `windows-2025` instead, and reported upstream |
| `lycheeverse/*`, `zizmorcore/*` as `ref-pin` in `zizmor.yml` | Creates CodeQL `actions/unpinned-tag` alerts (#1 and #3 are open in the template). Reported upstream |
| `workflows.yml` without `GIT_CONFIG_*` | Prints Git's "Using 'master'" hint (template run 36615491200). Every workflow here sets it, and a policy test now requires it. Reported upstream; the csharp and python templates have the same gap in three workflows each |
| `scripts/check-pipeline-status.sh` (aggregate status job) | The repository has no required status checks to aggregate. The C# workflow already has `check-csharp-pipeline-status.mjs`. A C++ status job would add a job without changing what can fail |
| `changelog.d/`, `check_changelog_fragment.py`, `check_version_modification.py`, `bump_version.py`, `version_and_commit.py` | They change the release model, where the version is bumped by CI from fragments. Here the version lives in the nuspec, edited by hand, and the preflight already skips versions that are on nuget.org. Issue #150 is about CI correctness, not the release model |
| `simulate-fresh-merge.sh` | GitHub's `pull_request` event already tests the merge commit with `main`. The script guards the template's own auto-commit flow, which does not exist here |
| `gcovr.cfg`, Codecov | Coverage of a header-only concepts library: the "code" is concept definitions that are checked at compile time. Line coverage would measure the test file |
| `.clang-tidy`, cppcheck | Not run in this PR. Enabling either as a blocking check first needs a triage of its findings on these C++20 concept headers, and probably code changes. A non-blocking run would only add warnings, which is what issue #150 removes. Left for a follow-up |
| `vcpkg.json`, `packaging/vcpkg/*`, `test_package/`, `conanfile.py` | Distribution channels this repository does not publish to. Conan Center has its own recipe (`platform.interfaces`) |
| `check_file_size.py` | The largest tracked C++ file has 212 lines (`Platform.Interfaces.Tests.cpp`). There is nothing to guard yet |
| `links.yml` (lychee) | See "Template comparison" in `../analysis.md`: the only links are external, so an online check would add flaky false positives |
| `paths:` filters removed in favor of `detect_code_changes.py` | The template runs one big workflow and skips jobs inside it. This repository has a workflow per language, and `paths:` filters do the same job without starting a run |

## Upstream reports

See `../upstream/filed-issues.txt`.
