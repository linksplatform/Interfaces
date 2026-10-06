## Summary

`release.yml` publishes to nuget.org only with the long-lived `NUGET_API_KEY` secret. It does not support [NuGet trusted publishing](https://learn.microsoft.com/nuget/nuget-org/trusted-publishing) (OIDC through `NuGet/login`). Both sibling templates already publish with OIDC:

- the [js template](https://github.com/link-foundation/js-ai-driven-development-pipeline-template/blob/main/.github/workflows/release.yml) uses npm trusted publishing;
- the [python template](https://github.com/link-foundation/python-ai-driven-development-pipeline-template/blob/main/.github/workflows/release.yml) uses PyPI trusted publishing with `id-token: write`.

The preflight added for #51/#57 only checks that the key is present. As its own comment says, an expired key surfaces at the push as a raw 403.

## Real-world impact

In linksplatform/Interfaces, the `NUGET_TOKEN` secret was last updated on 2022-12-02 and has since expired. Every C# release since then fails with:

```
error: Response status code does not indicate success: 403 (The specified API key is invalid, has expired, or does not have permission to access the specified package.).
```

This comes from [run 36213154898](https://github.com/linksplatform/Interfaces/actions/runs/36213154898), log line 1060. nuget.org is stuck at `0.5.0` while the project is at `0.6.1`. With trusted publishing there would be no key to expire.

## Reproduction

Set `NUGET_API_KEY` to any expired or revoked key, then push a version bump to `main`. `release-preflight` passes, and `Publish to NuGet` fails with the 403 above and no remediation hint.

## Workaround

linksplatform/Interfaces#151 does the following:

- adds `id-token: write` and [`NuGet/login@8d196754b4036150537f80ac539e15c2f1028841 # v1.2.0`](https://github.com/NuGet/login), which is used when the `NUGET_USER` repository variable is set;
- falls back to the API key secret otherwise;
- in the preflight, reports `auth=trusted-publishing|api-key|none` and checks the flat-container index, so an already-published version is skipped;
- in the push script, turns a 403 into actionable guidance: renew the key or configure trusted publishing.

See [`preflight-nuget-release.mjs`](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/.github/scripts/preflight-nuget-release.mjs) and [`push-nuget-package.sh`](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/.github/scripts/push-nuget-package.sh).

## Suggested fix

```yaml
    permissions:
      contents: write
      id-token: write
    steps:
      - name: NuGet login (OIDC)
        id: nuget-login
        if: ${{ vars.NUGET_USER != '' }}
        uses: NuGet/login@8d196754b4036150537f80ac539e15c2f1028841 # v1.2.0
        with:
          user: ${{ vars.NUGET_USER }}
      - name: Publish to NuGet
        env:
          NUGET_API_KEY: ${{ steps.nuget-login.outputs.NUGET_API_KEY || secrets.NUGET_API_KEY }}
```

Also:

- update `preflight-credentials.mjs` so that `NUGET_USER` alone satisfies the release precondition;
- print a remediation hint when `dotnet nuget push` returns 403.
