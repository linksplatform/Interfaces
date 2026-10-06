# Reusable workflows use Node.js 20 actions, ubuntu-latest, @main references and unpinned, over-privileged jobs

## Summary

The reusable workflows (at `06a7067`) produce deprecation warnings on every run. They also depend on mutable references.

| Location | Problem |
|---|---|
| `cpp-test.yml`, `deploy-cpp.yml` | `actions/checkout@v1`, `egor-tensin/setup-gcc@v1` and `ncipollo/release-action@v1.11.2` target Node.js 20. GitHub now forces Node.js 24 and warns: *"Node.js 20 is deprecated. The following actions target Node.js 20 but are being forced to run on Node.js 24: ncipollo/release-action@v1.11.2"* ([run 36213155523](https://github.com/linksplatform/Interfaces/actions/runs/36213155523)). See the [changelog](https://github.blog/changelog/2025-09-19-deprecation-of-node-20-on-github-actions-runners/). |
| all | `runs-on: ubuntu-latest`. This silently moved to 24.04, which broke `apt-get install nuget`. |
| `deploy-cpp.yml` | Calls the nested reusable workflows by `@main`. Callers cannot pin a version they have tested. |
| all | No top-level `permissions`, no `timeout-minutes`, and `actions/checkout` persists credentials. [zizmor](https://docs.zizmor.sh/) reports `excessive-permissions`, `artipacked`, `template-injection` and `unpinned-uses`. |
| commented-out `csharp.yml` | Still references `tj-actions/changed-files@v21`, which is affected by [CVE-2025-30066](https://github.com/advisories/GHSA-mrrh-fwg8-r2c3). |

## Reproduction

```sh
git clone https://github.com/linksplatform/Workflows && cd Workflows
docker run --rm -v "$PWD:/repo:ro" -w /repo rhysd/actionlint:1.7.12          # exit 1
docker run --rm -v "$PWD:/repo:ro" -w /repo ghcr.io/zizmorcore/zizmor:1.29.0 --offline .github  # exit 14
```

At `06a7067`, zizmor reports `114 findings (34 suppressed, 48 unsafe fixes): 19 informational, 12 low, 15 medium, 34 high`. The unsuppressed findings are 23 `unpinned-uses`, 40 `template-injection`, 10 `artipacked`, 5 `excessive-permissions` and 2 `superfluous-actions`.

## Workaround

linksplatform/Interfaces#151 moved all of its workflows in-repo and applied the following:

- `actions/*` at current majors (checkout v7, upload-artifact v7, download-artifact v8, setup-dotnet v6)
- `gh release create` instead of `ncipollo/release-action`
- `ubuntu-24.04`
- `persist-credentials: false`
- per-job timeouts
- least-privilege permissions

It also added a `Workflows` CI job that runs actionlint and zizmor, plus [repository policy tests](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/.github/scripts/workflow-policy.test.mjs).

## Suggested fix

- Apply the same changes here.
- Publish tags (`v1`, …) so callers can pin.
- Add the actionlint and zizmor workflow, so regressions fail in PRs.
