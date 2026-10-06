## Summary

On `main` (16bac38), the template's own CI carries three kinds of warning noise and two open CodeQL alerts:

1. **14 runner-label notices:** "The ubuntu-latest label will migrate to Ubuntu 26 beginning October 19, 2026" ([runner-images#14748](https://github.com/actions/runner-images/issues/14748)). Every job uses `runs-on: ubuntu-latest` (27 places). The test matrix in `release.yml:363-371` also uses `ubuntu-latest`, `macos-latest` and `windows-latest`.
2. **Two open CodeQL `actions/unpinned-tag` alerts:**
   - [#1](https://github.com/link-foundation/cpp-ai-driven-development-pipeline-template/security/code-scanning/1): `links.yml:52`, `lycheeverse/lychee-action@v2`.
   - [#3](https://github.com/link-foundation/cpp-ai-driven-development-pipeline-template/security/code-scanning/3): `workflows.yml:93`, `zizmorcore/zizmor-action@v0.6.4`.

   `.github/zizmor.yml` lists `lycheeverse/*` and `zizmorcore/*` as `ref-pin`, so zizmor accepts these tags while CodeQL flags them.
3. **`hint: Using 'master' as the name for the initial branch`** in all three jobs of `workflows.yml` (Lint Workflows, Audit Workflows, Pipeline Status). `workflows.yml` is the only workflow without the `GIT_CONFIG_COUNT`/`GIT_CONFIG_KEY_0`/`GIT_CONFIG_VALUE_0` env block that the other four have.

## Why it matters

- A floating `-latest` label changes the OS under an unchanged commit. In linksplatform/Interfaces, exactly that broke every C++ deploy: `ubuntu-latest` became 24.04, which has no `nuget` apt package (linksplatform/Workflows#6). Projects generated from this template inherit the same risk on October 19.
- The notices and hints hide real warnings, so a clean run can't be told apart from a noisy one.

## Reproduction

```bash
repo=link-foundation/cpp-ai-driven-development-pipeline-template
# Runner-label notices of a run (any run on main; 36615491200 is Workflows)
for job in $(gh api repos/$repo/actions/runs/36615491200/jobs --jq '.jobs[].id'); do
  gh api repos/$repo/check-runs/$job/annotations --jq '.[].message'
done
# Open CodeQL alerts
gh api repos/$repo/code-scanning/alerts --jq '.[] | select(.state=="open") | "\(.rule.id) \(.most_recent_instance.location.path):\(.most_recent_instance.location.start_line)"'
# Git hint
gh run view 36615491200 --repo $repo --log | grep "Using 'master'"
# Floating labels
git grep -nE "(runs-on|os): .*-latest" .github/workflows
```

## Workaround

None is needed for a correct build. Generated projects can pin the labels themselves.

## Suggested fix

- Pin the runner images: `ubuntu-24.04`, `macos-15`, `windows-2025`. Add a policy test that rejects `-latest` in `runs-on:` and `os:`, for example in `scripts/tests/test_workflows.py`, which already checks other workflow rules. GitHub prints a separate "macOS arm64 capacity" notice for `macos-15`; that one is outside the repository's control.
- Hash-pin `lycheeverse/lychee-action` and `zizmorcore/zizmor-action`, with the tag in a trailing comment. Move both namespaces from `ref-pin` to the `'*': hash-pin` default in `.github/zizmor.yml`, so zizmor and CodeQL agree. Dependabot keeps hash pins current.
- Add the `GIT_CONFIG_*` env block to `workflows.yml`.

linksplatform/Interfaces applied all three in [PR #151](https://github.com/linksplatform/Interfaces/pull/151), with a repository-wide policy test (`.github/scripts/workflow-policy.test.mjs`). Its CI is now down to the macOS capacity notice. Evidence: [`dev/log/issues/150/pulls/151/cpp-template/`](https://github.com/linksplatform/Interfaces/tree/issue-150-dfc4758d0022/dev/log/issues/150/pulls/151/cpp-template).

The csharp and python templates have the same Git hint in `docs.yml`, `security.yml` and `workflows.yml`. Their earlier fixes (csharp#35, python#28) covered only `release.yml`. They are reported separately.
