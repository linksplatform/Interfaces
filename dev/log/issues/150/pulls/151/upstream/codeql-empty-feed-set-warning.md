# C# buildless extraction logs "No NuGet feeds are reachable" when the repository has no nuget.config

## Problem

With `build-mode: none` for C#, a repository that has no `nuget.config` gets this warning in every analysis log, even though nuget.org is reachable and is used a moment later:

```
[build-stdout] [001] Found 0 nuget.config files in /home/runner/work/Interfaces/Interfaces.
[build-stdout] [001] Found 1 NuGet feeds (with inherited ones) in nuget.config files: https://api.nuget.org/v3/index.json
[build-stdout] [001] Checking NuGet feed reachability on feeds: 
[build-stdout] [001] Warning: No NuGet feeds are reachable.
...
[build-stdout] [001] Checking NuGet feed reachability on feeds: https://api.nuget.org/v3/index.json
[build-stdout] [001] Querying NuGet feed 'https://api.nuget.org/v3/index.json' succeeded.
[build-stdout] [001] Reachable NuGet feeds: https://api.nuget.org/v3/index.json
```

The list after "on feeds:" is empty. CodeQL CLI 2.27.1, `github/codeql-action@v4`, `ubuntu-24.04`. Full log: [run 37528763774, line 3483](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/dev/log/issues/150/pulls/151/ci-logs-after/1c4ba2d/run-37528763774.log#L3483). The same warning is in the earlier scheduled CodeQL runs of linksplatform/Interfaces, for example run 36213154910 (2026-09-26) and run 37194208178 (2026-10-04).

## Root cause

`NugetPackageRestorer.Restore()` always evaluates `feedManager.ReachableExplicitFeeds` when the responsiveness check is on. That calls `CheckSpecifiedFeeds(ExplicitFeeds)`, and `ExplicitFeeds` comes from the `nuget.config` files in the source tree, so it is empty here. `GetReachableNuGetFeeds` then warns whenever the result is empty, including when there was nothing to check ([`FeedManager.cs` on main](https://github.com/github/codeql/blob/main/csharp/extractor/Semmle.Extraction.CSharp.DependencyFetching/FeedManager.cs)):

```csharp
var reachableFeeds = feedsToCheck
    .Where(feed => feedManagerIo.IsFeedReachable(feed, initialTimeout, tryCount))
    .ToList();

if (reachableFeeds.Count == 0)
{
    logger.LogWarning($"No {fallbackStr}NuGet feeds are reachable.");
}
```

## Reproduction

1. Create a repository with any C# project that restores a package from nuget.org and has no `nuget.config`.
2. Run the default CodeQL setup or `github/codeql-action/init@v4` with `languages: csharp` and `build-mode: none`.
3. The `Perform CodeQL Analysis` log contains `Checking NuGet feed reachability on feeds: ` (empty) followed by `Warning: No NuGet feeds are reachable.`

## Workaround

A `nuget.config` does **not** help. With one, the explicit set is `{nuget.org}`, but the inherited set (`AllFeeds` minus `ExplicitFeeds`) is now empty, and `CheckSpecifiedFeeds(InheritedFeeds)` logs the same warning ([run 37530537674](https://github.com/linksplatform/Interfaces/actions/runs/37530537674)):

```
[build-stdout] [001] Found 1 nuget.config files in /home/runner/work/Interfaces/Interfaces: ...
[build-stdout] [001] Checking NuGet feed reachability on feeds: https://api.nuget.org/v3/index.json
[build-stdout] [001] Checking NuGet feed reachability on feeds: 
[build-stdout] [001] Warning: No NuGet feeds are reachable.
```

The only workaround is to turn the reachability check off for repositories whose only feed is nuget.org:

```yaml
- uses: github/codeql-action/analyze@v4
  env:
    CODEQL_EXTRACTOR_CSHARP_BUILDLESS_NUGET_FEEDS_CHECK: 'false'
```

linksplatform/Interfaces does this in [PR #151](https://github.com/linksplatform/Interfaces/pull/151).

## Suggested fix

Do not check or warn when there is nothing to check, for example at the start of `GetReachableNuGetFeeds`:

```csharp
if (feedsToCheck.Count == 0)
{
    logger.LogInfo($"No {fallbackStr}NuGet feeds to check for reachability.");
    return [];
}
```
