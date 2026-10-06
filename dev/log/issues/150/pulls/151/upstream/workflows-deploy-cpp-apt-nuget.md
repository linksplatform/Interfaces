# deploy-cpp.yml: apt-get install nuget fails on ubuntu-24.04, and publishRelease still creates the release

## Summary

`deploy-cpp.yml` (at `06a7067`) installs the NuGet CLI with `sudo apt-get install nuget`. `ubuntu-latest` is Ubuntu 24.04, and Ubuntu 24.04 no longer ships that package, so `pushToNuget` fails with exit code 100. The `publishRelease` job does not depend on `pushToNuget`, so it still creates the GitHub release. The result is a release page that links to a NuGet package that was never published.

## Evidence

From linksplatform/Interfaces run [36213155523](https://github.com/linksplatform/Interfaces/actions/runs/36213155523) (image `ubuntu24/20260920.314`):

```text
main / pushToNuget  Package nuget is not available, but is referred to by another package.
main / pushToNuget  E: Package 'nuget' has no installation candidate
main / pushToNuget  ##[error]Process completed with exit code 100.
```

The same run still created release [`cpp_0.4.1`](https://github.com/linksplatform/Interfaces/releases/tag/cpp_0.4.1) at 02:55:28Z. The releases `cpp_0.3.43` and `cpp_0.4.0` were created the same way. Meanwhile [nuget.org](https://api.nuget.org/v3-flatcontainer/platform.interfaces.templatelibrary/index.json) stops at `0.3.41`.

## Minimal reproduction

```sh
docker run --rm ubuntu:24.04 sh -c 'apt-get update -qq && apt-cache policy nuget && apt-get install -y nuget'
# nuget:
#   Installed: (none)
#   Candidate: (none)
# E: Unable to locate package nuget      (exit 100)
```

## Workaround

linksplatform/Interfaces#151 stops calling the reusable workflow and does the following instead:

- packs the `.nuspec` with `dotnet pack`, which the runner image already has, through a tiny project that sets `NuspecFile`, `NuspecBasePath` and `NoBuild`;
- uploads the `.nupkg` as an artifact from the test workflow;
- pushes with `dotnet nuget push --skip-duplicate`;
- creates the release with `needs: [test, pushToNuget]`, so the release is never created without the package.

The scripts involved are [`pack-cpp-nuget.mjs`](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/.github/scripts/pack-cpp-nuget.mjs) and [`push-nuget-package.sh`](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/.github/scripts/push-nuget-package.sh).

## Suggested fix

1. Replace `apt-get install nuget` and `nuget pack` / `nuget push` with `dotnet pack` (or `NuGet/setup-nuget` pinned by commit) and `dotnet nuget push --skip-duplicate`.
2. Add `pushToNuget` to `publishRelease.needs`. Do not create the release when the push failed.
3. Before pushing, check whether the version is already on nuget.org (`https://api.nuget.org/v3-flatcontainer/<id>/index.json`). This keeps a re-run idempotent.
4. Pin `runs-on: ubuntu-24.04`, so a future image migration fails in a PR instead of in production.
