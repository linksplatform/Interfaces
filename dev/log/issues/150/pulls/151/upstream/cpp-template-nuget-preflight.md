## Summary

`scripts/preflight-credentials.sh` treats HTTP 200 from `POST /api/v2/package/create-verification-key/{id}` as proof that `NUGET_API_KEY` "may push" `NUGET_PACKAGE_ID` (lines 15-17 and 160-162). nuget.org does not check the package id at that step. A valid, unexpired push key whose glob or owner does not cover the package still gets 200. The preflight then reports `ok`, and the release fails later at `dotnet nuget push` with 403. That is a false positive.

The release also has no [NuGet trusted publishing](https://learn.microsoft.com/nuget/nuget-org/trusted-publishing) path. A long-lived `NUGET_API_KEY` is the only option, and nuget.org keys expire after at most 365 days. This is the same gap as csharp-template#66.

## Evidence (NuGetGallery source)

At [NuGet/NuGetGallery@2f271fb, `ApiController.cs`](https://github.com/NuGet/NuGetGallery/blob/2f271fb8651906ecd841d0a658f9afc670e7021d/src/NuGetGallery/Controllers/ApiController.cs#L328-L352):

```csharp
[ApiScopeRequired(NuGetScopes.PackagePush, NuGetScopes.PackagePushVersion)]
public virtual async Task<ActionResult> CreatePackageVerificationKeyAsync(string id, string version)
{
    // ... we always create a temp key scoped to the unverified package ID here and defer package and owner
    // validation until the VerifyPackageKey call.
```

`ApiScopeRequiredAttribute` only calls `identity.HasScopeThatAllowsActions(ScopeActions)`, which checks the actions and not the subject.

The id, glob and owner check happens in `GET /api/v2/verifykey/{id}/{version}` (`VerifyPackageKeyInternalAsync`, lines 383-414). It runs with the one-time key, against an existing version, and returns 200, 403, or 404 when that version does not exist.

What step 1 does detect: an invalid or expired key (403, as with `Platform.Interfaces` in linksplatform/Interfaces), or a key without any push scope.

## Reproduction

1. On nuget.org, create an API key with *Push* scope and a glob that does not match the package, for example `Some.Other.*`.
2. Run the preflight with `NUGET_PUBLISH=true NUGET_PACKAGE_ID=<your package> NUGET_API_KEY=<that key> bash scripts/preflight-credentials.sh`. NuGet reports `ok`.
3. `dotnet nuget push` with the same key returns 403.

(We could not run this end to end without a second account's key. The behaviour follows from the source above. The expired-key 403 was reproduced live: [probe log](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/dev/log/issues/150/pulls/151/nuget-verification-key-probe.log).)

## Workaround

None inside the preflight. The push step reports the 403.

## Suggested fix

1. Finish the flow the NuGet client uses. After the 200, call `GET {gallery}/api/v2/verifykey/{id}/{latest published version}` with header `X-NuGet-ApiKey: <returned Key>`:
   - 200: OK.
   - 401/403: fail.
   - 404 or no published version: report `unknown` for the package scope, not `ok`. A first push can't be checked this way.

   The one-time key is deleted by the gallery after this call. The empty POST also needs `Content-Length: 0`, which the script already sends; without it, nuget.org answers 411.
2. Support trusted publishing. Add `permissions: id-token: write` on the publishing job, then `NuGet/login` (hash-pinned) with `user: ${{ vars.NUGET_USER }}`, and pass its `NUGET_API_KEY` output to `dotnet nuget push`. Keep the secret as a fallback, and have the preflight report which mode is used.

linksplatform/Interfaces implements both in [PR #151](https://github.com/linksplatform/Interfaces/pull/151): `.github/scripts/preflight-nuget-release.mjs` (`verifyApiKey`), with tests in `preflight-nuget-release.test.mjs`, and `deploy-cpp.yml`/`csharp.yml` for `NuGet/login`.
